#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ABTesterAudioProcessorEditor  : public gin::ProcessorEditor
{
public:
    ABTesterAudioProcessorEditor (ABTesterAudioProcessor&);
    ~ABTesterAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    ABTesterAudioProcessor& abProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ABTesterAudioProcessorEditor)
};
