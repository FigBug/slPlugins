#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CrossfeedAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    CrossfeedAudioProcessorEditor (CrossfeedAudioProcessor&);
    ~CrossfeedAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    CrossfeedAudioProcessor& proc;
    
    gin::LevelMeter meter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfeedAudioProcessorEditor)
};
