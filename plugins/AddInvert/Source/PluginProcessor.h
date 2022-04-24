#pragma once

#include <JuceHeader.h>

class AddInvertAudioProcessorEditor;
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
    
private:
    juce::Component::SafePointer<AddInvertAudioProcessorEditor> editor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddInvertAudioProcessor)
};
