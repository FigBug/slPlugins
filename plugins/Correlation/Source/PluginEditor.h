#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CorrelationDisplay.h"

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
    void addSpeedItems (juce::PopupMenu&);
    void showSpeedMenu();

    PluginProcessor& proc;

    CorrelationDisplay display { proc.meter };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
