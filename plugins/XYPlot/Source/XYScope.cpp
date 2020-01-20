#include "XYScope.h"

XYScope::XYScope (TimeSliceThread* tst) :
    backgroundThreadToUse (tst, tst == nullptr),
    needToUpdate (false),
    needToRepaint (true)
{
    setColour (lineColourId, Colours::white);
    setColour (backgroundColourId, Colours::black);
    setColour (traceColourId, Colours::white);
    
    const ScopedLock sl (imageLock);
    image = Image (Image::RGB, jmax (1, getWidth()), jmax (1, getHeight()), true);
    Graphics g (image);
    g.fillAll (Colours::transparentBlack);

    if (backgroundThreadToUse == nullptr)
    {
        OptionalScopedPointer<TimeSliceThread> newThread (new TimeSliceThread ("Triggered Scope Rendering Thread"), true);
        backgroundThreadToUse = newThread;
        backgroundThreadToUse->startThread (1);
    }

    backgroundThreadToUse->addTimeSliceClient (this);

    for (auto c : channels)
    {
        c->minBuffer.clear ((size_t) c->bufferSize);
        c->maxBuffer.clear ((size_t) c->bufferSize);
    }

    startTimer (1000 / 60);
}

XYScope::~XYScope()
{
    const ScopedLock sl (imageLock);

    stopTimer();

    backgroundThreadToUse->removeTimeSliceClient (this);

    if (backgroundThreadToUse.willDeleteObject())
        backgroundThreadToUse->stopThread (500);
}

void XYScope::addSamples (const AudioSampleBuffer& buffer)
{
    jassert (buffer.getNumChannels() == channels.size());
    
    for (int i = 0; i < 2; i++)
    {
        const float* samples = buffer.getReadPointer (i);
        const int numSamples = buffer.getNumSamples();
        
        // if we don't have enough space in the fifo, clear out some old samples
        const int numFreeInBuffer = channels[i]->samplesToProcess.getNumFree();
        if (numFreeInBuffer < numSamples)
            channels[i]->samplesToProcess.removeSamples (numFreeInBuffer);
        
        channels[i]->samplesToProcess.writeSamples (samples, numSamples);
    }
    needToUpdate = true;
}

//==============================================================================
void XYScope::resized()
{
    const ScopedLock sl (imageLock);

    image = Image (Image::ARGB, jmax (1, getWidth()), jmax (1, getHeight()), true);
    Graphics g (image);
    g.fillAll (findColour (backgroundColourId));

    needToRepaint = true;
}

void XYScope::paint (Graphics& g)
{
    const ScopedLock sl (imageLock);

    g.drawImageAt (image, 0, 0);
    
    g.setColour (findColour (lineColourId));
    g.drawRect (getLocalBounds());
}

void XYScope::timerCallback()
{
    if (needToRepaint)
        repaint();
}

int XYScope::useTimeSlice()
{
    if (needToUpdate)
    {
        processPendingSamples();
        renderImage();

        needToUpdate = false;
    }

    return 10;
}

//==============================================================================
void XYScope::processPendingSamples()
{
    for (auto c : channels)
    {
        int numSamples = c->samplesToProcess.getNumAvailable();
        c->samplesToProcess.readSamples (c->tempProcessingBlock, numSamples);
        float* samples = c->tempProcessingBlock.getData();

        while (--numSamples >= 0)
        {
            const float currentSample = *samples++;

            if (currentSample < c->currentMin)
                c->currentMin = currentSample;
            if (currentSample > c->currentMax)
                c->currentMax = currentSample;

            if (--c->numLeftToAverage == 0)
            {
                c->minBuffer[c->bufferWritePos] = c->currentMin;
                c->maxBuffer[c->bufferWritePos] = c->currentMax;

                c->currentMax = -1.0f;
                c->currentMin = 1.0f;

                ++c->bufferWritePos %= c->bufferSize;
                c->numLeftToAverage = numSamplesPerPixel;
            }
        }
    }
}

void XYScope::renderImage()
{
    const ScopedLock sl (imageLock);

    image.clear ({0, 0, image.getWidth(), image.getHeight()}, Colours::transparentBlack);
    
    Graphics g (image);

    g.fillAll (Colours::transparentBlack);

    const int w = image.getWidth();
    const int h = image.getHeight();

    
    
    int bufferReadPos = getTriggerPos();
    
    bufferReadPos -= roundFloatToInt (w * triggerPos);
    if (bufferReadPos < 0 )
        bufferReadPos += channels[0]->bufferSize;
    
    int ch = 0;
    for (auto c : channels)
    {
        g.setColour (findColour (traceColourId + ch));
        
        int pos = bufferReadPos;
        int currentX = 0;
        Range<float> r0;
        while (currentX < w)
        {
            ++pos;
            if (pos == c->bufferSize)
                pos = 0;

            const float top = (1.0f - (0.5f + (0.5f * verticalZoomFactor * (verticalZoomOffset[ch] + c->maxBuffer[pos])))) * h;
            const float bottom = (1.0f - (0.5f + (0.5f * verticalZoomFactor * (verticalZoomOffset[ch] + c->minBuffer[pos])))) * h;
            
            //jassert (top <= bottom);
            
            Range<float> r1 (top, bottom);

            if (currentX == 0 || r1.intersects(r0))
                g.drawVerticalLine (currentX, top, bottom);
            else if (r0.getEnd() < r1.getStart())
                g.drawLine (currentX - 1.0f, float (r0.getEnd()), float (currentX), float (r1.getEnd()));
            else if (r0.getStart() > r1.getEnd())
                g.drawLine (currentX - 1.0f, r0.getStart(), float (currentX), r1.getStart());
            else
                g.drawLine (currentX - 1.0f, top, float (currentX), bottom);
            
            ++currentX;
            r0 = r1;
        }
        ch++;
    }

    needToRepaint = true;
}
