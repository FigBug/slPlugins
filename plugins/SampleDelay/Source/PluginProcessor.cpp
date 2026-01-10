#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

static juce::String modeTextFunction (const gin::Parameter&, float v)
{
	return v == 0.0f ? "Samples" : "Time";
}

static juce::String enableTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    gin::ProcessorOptions opts;
    opts.hasMidiLearn = true;
    return opts;
}

SampleDelayAudioProcessor::SampleDelayAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    mode     = addExtParam ("mode",     "Mode",      "", "",   {   0.0f,     1.0f, 1.0f, 1.0f},    0.0f, 0.0f, modeTextFunction);
	link     = addExtParam ("link",     "Link",      "", "",   {   0.0f,     1.0f, 1.0f, 1.0f},    1.0f, 0.0f, enableTextFunction);
	time     = addExtParam ("time",     "Time",    "", "ms", {   0.0f,  1000.0f, 0.0f, 1.0f},    1.0f, {0.2f, gin::SmoothingType::eased});
	samples  = addExtParam ("samples",  "Samples", "", "",   {   0.0f, 44100.0f, 1.0f, 1.0f},   50.0f, {0.2f, gin::SmoothingType::eased});
	timeL    = addExtParam ("timel",    "Time L",    "", "ms", {   0.0f,  1000.0f, 0.0f, 1.0f},    1.0f, {0.2f, gin::SmoothingType::eased});
	samplesL = addExtParam ("samplesl", "Samples L", "", "",   {   0.0f, 44100.0f, 1.0f, 1.0f},   50.0f, {0.2f, gin::SmoothingType::eased});
	timeR    = addExtParam ("timer",    "Time R",    "", "ms", {   0.0f,  1000.0f, 0.0f, 1.0f},    1.0f, {0.2f, gin::SmoothingType::eased});
	samplesR = addExtParam ("samplesr", "Samples R", "", "",   {   0.0f, 44100.0f, 1.0f, 1.0f},   50.0f, {0.2f, gin::SmoothingType::eased});

    timeL->conversionFunction = [] (float in) { return in / 1000.0f; };
	timeR->conversionFunction = [] (float in) { return in / 1000.0f; };

    init();
}

SampleDelayAudioProcessor::~SampleDelayAudioProcessor()
{
}

//==============================================================================
void SampleDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
    
    int ch = std::max (getTotalNumInputChannels(), getTotalNumOutputChannels());
    double sr = getSampleRate();
     
    delayLine.setSize (ch, std::max (1.1, sr / 44100.0 + 0.1), sr);
}

void SampleDelayAudioProcessor::reset()
{
    gin::Processor::reset();
    
    delayLine.clear();
}

void SampleDelayAudioProcessor::numChannelsChanged ()
{
    int ch = getTotalNumInputChannels();
    double sr = getSampleRate();
    
    delayLine.setSize (ch, std::max (1.1, sr / 44100.0 + 0.1), sr);
}

void SampleDelayAudioProcessor::releaseResources()
{
}

void SampleDelayAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
    int ch = buffer.getNumChannels();
    
    gin::ScratchBuffer scratch (ch, numSamples);
    
    for (int s = 0; s < numSamples; s++)
    {
		auto sampPos = samples->getProcValue (1);
		auto timePos = time->getProcValue (1);

		for (int c = 0; c < ch; c++)
        {
			if (! link->getBoolValue())
			{
				sampPos = c == 0 ? samplesL->getProcValue (1) : samplesR->getProcValue (1);
				timePos = c == 0 ? timeL->getProcValue (1)    : timeR->getProcValue (1);
			}

            if (mode->getUserValueInt() == 0)
            {
                if (sampPos == 0)
                    *scratch.getWritePointer (c, s) = *buffer.getReadPointer (c, s);
                else
                    *scratch.getWritePointer (c, s) = delayLine.readSample (c, int (sampPos));
            }
            else
            {
                if (timePos == 0)
                    *scratch.getWritePointer (c, s) = *buffer.getReadPointer (c, s);
                else
                    *scratch.getWritePointer (c, s) = delayLine.readLinear (c, timePos);
            }
        
            delayLine.write (c, *buffer.getReadPointer (c, s));
        }
        delayLine.writeFinished();
    }
    
    for (int c = 0; c < ch; c++)
        buffer.copyFrom (c, 0, scratch, c, 0, numSamples);
}

//==============================================================================
bool SampleDelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SampleDelayAudioProcessor::createEditor()
{
    return new SampleDelayAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleDelayAudioProcessor();
}
