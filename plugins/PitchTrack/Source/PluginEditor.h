#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PitchTrackAudioProcessorEditor : public gin::ProcessorEditor,
                                       private juce::Timer
{
public:
    PitchTrackAudioProcessorEditor (PitchTrackAudioProcessor&);
    ~PitchTrackAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint (juce::Graphics& g) override;
    void timerCallback() override;

private:
    PitchTrackAudioProcessor& proc;
    
    float lastPitch = -1;
    juce::Label pitch;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchTrackAudioProcessorEditor)
};
