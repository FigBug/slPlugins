/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ChannelMuteAudioProcessorEditor  : public gin::GinAudioProcessorEditor
{
public:
    ChannelMuteAudioProcessorEditor (ChannelMuteAudioProcessor&);
    ~ChannelMuteAudioProcessorEditor();

    //==============================================================================
    void resized() override;

    drow::TriggeredScope scope;

private:
    ChannelMuteAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelMuteAudioProcessorEditor)
};
