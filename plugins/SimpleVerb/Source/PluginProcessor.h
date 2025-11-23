#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    AudioProcessor();
    ~AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::Parameter::Ptr room, damp, preDelay, lp, hp, wet, dry;

private:
    
    gin::SimpleVerb reverb;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor)
};
