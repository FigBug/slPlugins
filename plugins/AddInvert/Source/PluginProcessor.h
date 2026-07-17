#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AddInvertAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    AddInvertAudioProcessor();
    ~AddInvertAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::AudioFifo fifo { 2, 44100 };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddInvertAudioProcessor)
};
