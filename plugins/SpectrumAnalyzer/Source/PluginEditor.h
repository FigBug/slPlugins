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

    drow::Spectroscope scopeL {10};
    drow::Spectroscope scopeR {10};
    
    drow::Sonogram sonogramL {10};
    drow::Sonogram sonogramR {10};

private:
    juce::CriticalSection lock;
    void updateScope();
    void valueUpdated (gin::Parameter*) override { updateScope(); }
    
    PluginProcessor& proc;
    
    juce::TimeSliceThread thread { "timeSlice" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
