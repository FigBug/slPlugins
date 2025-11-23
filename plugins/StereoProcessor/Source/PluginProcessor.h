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
    gin::Parameter::Ptr width1, center1, pan1, rotation, pan2, center2, width2, output;

private:
    
    gin::StereoProcessor proc;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor)
};
