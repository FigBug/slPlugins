#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicEQAudioProcessor::GraphicEQAudioProcessor()
    : gin::Processor (BusesProperties()
                        .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true),
                      false,
                      gin::ProcessorOptions().withMidiLearn())
{
    fifo.setSize (2, 44100);

    // Band gain parameters with 0.1s smoothing
    const char* bandNames[] = { "31", "63", "125", "250", "500", "1k", "2k", "4k", "8k", "16k" };

    for (int i = 0; i < numBands; i++)
    {
        auto id = juce::String ("band") + juce::String (i);
        auto name = juce::String (bandNames[i]) + " Hz";
        bandGain[static_cast<size_t> (i)] = addExtParam (id, name, "", "dB", { -12.0f, 12.0f, 0.0f, 1.0f }, 0.0f, 0.1f);
    }

    // Text function to display frequency in Hz from MIDI note number
    auto freqTextFunction = [] (const gin::Parameter&, float v)
    {
        auto freq = gin::getMidiNoteInHertz (v);
        if (freq >= 1000.0f)
            return juce::String (freq / 1000.0f, 1) + " kHz";
        return juce::String (juce::roundToInt (freq)) + " Hz";
    };

    // Text function for enable buttons
    auto enableTextFunction = [] (const gin::Parameter&, float v)
    {
        return v > 0.5f ? juce::String ("On") : juce::String ("Off");
    };

    // Text function for slope buttons
    auto slopeTextFunction = [] (const gin::Parameter&, float v)
    {
        return v > 0.5f ? juce::String ("24 dB") : juce::String ("12 dB");
    };

    // LP filter parameters - frequency in MIDI note numbers
    // 1000 Hz ~ note 83, 20000 Hz ~ note 135
    auto lpMinNote = gin::getMidiNoteFromHertz (1000.0f);
    auto lpMaxNote = gin::getMidiNoteFromHertz (20000.0f);
    auto lpDefaultNote = gin::getMidiNoteFromHertz (20000.0f);

    lpEnable = addExtParam ("lpEnable", "LP", "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, enableTextFunction);
    lpFreq   = addExtParam ("lpFreq",   "LP Freq",   "", "", { lpMinNote, lpMaxNote, 0.0f, 1.0f }, lpDefaultNote, 0.1f, freqTextFunction);
    lpSlope  = addExtParam ("lpSlope",  "LP Slope",  "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, slopeTextFunction);

    lpFreq->conversionFunction = [] (float in) { return gin::getMidiNoteInHertz (in); };

    // HP filter parameters - frequency in MIDI note numbers
    // 20 Hz ~ note 23, 2000 Hz ~ note 107
    auto hpMinNote = gin::getMidiNoteFromHertz (20.0f);
    auto hpMaxNote = gin::getMidiNoteFromHertz (2000.0f);
    auto hpDefaultNote = gin::getMidiNoteFromHertz (20.0f);

    hpEnable = addExtParam ("hpEnable", "HP", "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, enableTextFunction);
    hpFreq   = addExtParam ("hpFreq",   "HP Freq",   "", "", { hpMinNote, hpMaxNote, 0.0f, 1.0f }, hpDefaultNote, 0.1f, freqTextFunction);
    hpSlope  = addExtParam ("hpSlope",  "HP Slope",  "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f, slopeTextFunction);

    hpFreq->conversionFunction = [] (float in) { return gin::getMidiNoteInHertz (in); };

    // Output gain with 0.1s smoothing
    outputGain = addExtParam ("output", "Output", "", "dB", { -12.0f, 12.0f, 0.0f, 1.0f }, 0.0f, 0.1f);
    outputGain->conversionFunction = [] (float in) { return juce::Decibels::decibelsToGain (in); };

    init();
}

GraphicEQAudioProcessor::~GraphicEQAudioProcessor()
{
}

//==============================================================================
void GraphicEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    eq.setSampleRate (sampleRate);
    eq.setNumChannels (2);
    eq.setBlockSize (samplesPerBlock);
    eq.reset();

    lpFilter.setSampleRate (sampleRate);
    lpFilter.setNumChannels (2);
    lpFilter.setType (gin::Filter::lowpass);
    lpFilter.reset();

    hpFilter.setSampleRate (sampleRate);
    hpFilter.setNumChannels (2);
    hpFilter.setType (gin::Filter::highpass);
    hpFilter.reset();
}

void GraphicEQAudioProcessor::reset()
{
    gin::Processor::reset();

    eq.reset();
    lpFilter.reset();
    hpFilter.reset();
}

void GraphicEQAudioProcessor::releaseResources()
{
}

void GraphicEQAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Copy input to fifo channel 0 (pre-filter spectrum)
    gin::ScratchBuffer fifoData (2, numSamples);
    if (numChannels >= 2)
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (0), buffer.getReadPointer (0), numSamples);
        juce::FloatVectorOperations::add (fifoData.getWritePointer (0), buffer.getReadPointer (1), numSamples);
        juce::FloatVectorOperations::multiply (fifoData.getWritePointer (0), 0.5f, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (0), buffer.getReadPointer (0), numSamples);
    }

    // Check if we need sample-by-sample processing for smoothing
    if (isSmoothing())
    {
        for (int pos = 0; pos < numSamples; pos++)
        {
            // Update EQ band gains
            for (int b = 0; b < numBands; b++)
            {
                float gain = bandGain[static_cast<size_t> (b)]->getProcValue (1);
                eq.setBandGain (0, b, gain);
                eq.setBandGain (1, b, gain);
            }

            // Process single sample through EQ
            auto sampleBuffer = gin::sliceBuffer (buffer, pos, 1);
            eq.process (sampleBuffer);

            // HP filter
            if (hpEnable->getProcValue (1) > 0.5f)
            {
                hpFilter.setSlope (hpSlope->getProcValue (1) > 0.5f ? gin::Filter::db24 : gin::Filter::db12);
                hpFilter.setParams (hpFreq->getProcValue (1), 0.707f);
                hpFilter.process (sampleBuffer);
            }

            // LP filter
            if (lpEnable->getProcValue (1) > 0.5f)
            {
                lpFilter.setSlope (lpSlope->getProcValue (1) > 0.5f ? gin::Filter::db24 : gin::Filter::db12);
                lpFilter.setParams (lpFreq->getProcValue (1), 0.707f);
                lpFilter.process (sampleBuffer);
            }

            // Output gain
            float gain = outputGain->getProcValue (1);
            for (int ch = 0; ch < numChannels; ch++)
                buffer.getWritePointer (ch)[pos] *= gain;
        }
    }
    else
    {
        // Update EQ band gains for block processing
        for (int b = 0; b < numBands; b++)
        {
            float gain = bandGain[static_cast<size_t> (b)]->getProcValue (numSamples);
            eq.setBandGain (0, b, gain);
            eq.setBandGain (1, b, gain);
        }

        // Process entire block
        eq.process (buffer);

        // HP filter
        if (hpEnable->getProcValue (numSamples) > 0.5f)
        {
            hpFilter.setSlope (hpSlope->getProcValue (numSamples) > 0.5f ? gin::Filter::db24 : gin::Filter::db12);
            hpFilter.setParams (hpFreq->getProcValue (numSamples), 0.707f);
            hpFilter.process (buffer);
        }

        // LP filter
        if (lpEnable->getProcValue (numSamples) > 0.5f)
        {
            lpFilter.setSlope (lpSlope->getProcValue (numSamples) > 0.5f ? gin::Filter::db24 : gin::Filter::db12);
            lpFilter.setParams (lpFreq->getProcValue (numSamples), 0.707f);
            lpFilter.process (buffer);
        }

        // Output gain
        float gain = outputGain->getProcValue (numSamples);
        buffer.applyGain (gain);
    }

    // Copy output to fifo channel 1 (post-filter spectrum)
    if (numChannels >= 2)
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (1), buffer.getReadPointer (0), numSamples);
        juce::FloatVectorOperations::add (fifoData.getWritePointer (1), buffer.getReadPointer (1), numSamples);
        juce::FloatVectorOperations::multiply (fifoData.getWritePointer (1), 0.5f, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (1), buffer.getReadPointer (0), numSamples);
    }

    if (fifo.getFreeSpace() >= numSamples)
        fifo.write (fifoData);
}

//==============================================================================
bool GraphicEQAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* GraphicEQAudioProcessor::createEditor()
{
    return new GraphicEQAudioProcessorEditor (*this);
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraphicEQAudioProcessor();
}
