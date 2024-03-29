/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UIComponents.h"

//==============================================================================
/**
*/
class SFXAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    SFXAudioProcessorEditor (SFXAudioProcessor&);
    ~SFXAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint (juce::Graphics& g) override;
    void refresh();

private:    
    SFXAudioProcessor& sfxProcessor;

    PadGridComponent padGrid {sfxProcessor};
    ParamComponent params {sfxProcessor};
    juce::Image logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SFXAudioProcessorEditor)
};
