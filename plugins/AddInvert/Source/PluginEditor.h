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

private:
    AddInvertAudioProcessor& aiProcessor;
    gin::TriggeredScope scope { aiProcessor.fifo };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddInvertAudioProcessorEditor)
};
