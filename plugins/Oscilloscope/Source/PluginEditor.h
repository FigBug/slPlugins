#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PluginEditor  : public gin::ProcessorEditor,
                      private gin::Parameter::ParameterListener,
                      private juce::Timer
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void resized() override;
    juce::Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1) override;

private:
    juce::CriticalSection lock;
    void updateScope();
    void valueUpdated (gin::Parameter*) override { updateScope(); }

    void timerCallback() override;
    void updateRecordButtons();
    void updateRecordButtonColours();
    void showRecordMenu();

    PluginProcessor& scopeProc;

    gin::TriggeredScope scope { scopeProc.fifo };

    gin::SVGButton recordNormal;
    gin::SVGButton recordTriggered;
    gin::SVGButton recordRetrospective;
    gin::SVGButton recordStop;
    gin::SVGButton recordMenu;
    bool flashState = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
