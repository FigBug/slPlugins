#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LoudnessDisplay.h"

//==============================================================================
/**
*/
class PluginEditor  : public gin::ProcessorEditor
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void resized() override;
    void addMenuItems (juce::PopupMenu&) override;

private:
    void addScaleItems (juce::PopupMenu&);
    void showScaleMenu();

    PluginProcessor& proc;

    LoudnessDisplay display { proc };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
