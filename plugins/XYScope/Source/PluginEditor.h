#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PluginEditor  : public gin::ProcessorEditor,
                      private gin::Parameter::ParameterListener
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void resized() override;
    Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1) override;

private:
    CriticalSection lock;
    void updateScope();
    void valueUpdated (gin::Parameter*) override { updateScope(); }
    
    PluginProcessor& proc;
    gin::XYScope scope { proc.fifo };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
