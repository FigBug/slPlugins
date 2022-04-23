#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class LimiterAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    LimiterAudioProcessor();
    ~LimiterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    void numChannelsChanged() override;

    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::Parameter::Ptr attack, release, threshold, input, output;

    gin::Dynamics limiter;
    gin::AudioFifo fifo;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LimiterAudioProcessor)
};
