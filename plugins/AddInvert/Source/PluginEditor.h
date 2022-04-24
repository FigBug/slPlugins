#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AddInvertAudioProcessorEditor  : public gin::ProcessorEditor
{
public:
    AddInvertAudioProcessorEditor (AddInvertAudioProcessor&);
    ~AddInvertAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    
    drow::TriggeredScope scope;

private:
    AddInvertAudioProcessor& aiProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddInvertAudioProcessorEditor)
};
