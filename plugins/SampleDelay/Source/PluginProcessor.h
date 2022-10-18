#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SampleDelayAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    SampleDelayAudioProcessor();
    ~SampleDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::Parameter::Ptr mode, link, time, samples, timeL, samplesL, timeR, samplesR;

private:
    void numChannelsChanged () override;
    
    gin::DelayLine delayLine { 2, 2 };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleDelayAudioProcessor)
};
