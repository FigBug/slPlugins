#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class EQ3AudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    EQ3AudioProcessor();
    ~EQ3AudioProcessor() override;

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

    gin::Dynamics gate;
    gin::AudioFifo fifo;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQ3AudioProcessor)
};
