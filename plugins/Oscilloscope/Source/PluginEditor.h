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
    juce::Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1) override;

private:
    juce::CriticalSection lock;
    void updateScope();
    void parameterChanged (gin::Parameter*) override { updateScope(); }
    
    PluginProcessor& scopeProc;

    gin::TriggeredScope scope { scopeProc.fifo };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
