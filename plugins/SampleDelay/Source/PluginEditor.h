#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SampleDelayAudioProcessorEditor : public gin::ProcessorEditor,
                                        private gin::Parameter::ParameterListener
{
public:
    SampleDelayAudioProcessorEditor (SampleDelayAudioProcessor&);
    ~SampleDelayAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    void valueUpdated (gin::Parameter* param) override;
    
    SampleDelayAudioProcessor& proc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleDelayAudioProcessorEditor)
};
