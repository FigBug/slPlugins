#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class PluginEditor;

#define PARAM_MODE     "mode"
#define PARAM_LOG      "log"

//==============================================================================
/**
*/
class PluginProcessor : public gin::Processor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================

    juce::CriticalSection lock;
    PluginEditor* editor = nullptr;

private:    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
