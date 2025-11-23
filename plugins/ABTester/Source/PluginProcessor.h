#pragma once

#include <JuceHeader.h>

using namespace juce::dsp;


#define PARAM_AB        "ab"
#define PARAM_LEVEL     "level"

class ABTesterAudioProcessorEditor;
//==============================================================================
/**
*/
class ABTesterAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    ABTesterAudioProcessor();
    ~ABTesterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    
private:
    juce::Component::SafePointer<ABTesterAudioProcessorEditor> editor;

    juce::LinearSmoothedValue<float> aVal;
    juce::LinearSmoothedValue<float> bVal;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ABTesterAudioProcessor)
};
