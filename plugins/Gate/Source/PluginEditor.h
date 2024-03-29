/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GateAudioProcessorEditor : public gin::ProcessorEditor,
                                 private gin::Parameter::ParameterListener
{
public:
    GateAudioProcessorEditor (GateAudioProcessor&);
    ~GateAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void valueUpdated (gin::Parameter* param) override;

private:
    GateAudioProcessor& proc;

    gin::DynamicsMeter meter {proc.gate};
    gin::TriggeredScope scope {proc.fifo};

    gin::LevelMeter inputMeter {proc.gate.getInputTracker()};
    gin::LevelMeter outputMeter {proc.gate.getOutputTracker()};
    gin::LevelMeter reductionMeter {proc.gate.getReductionTracker(), {-30, 0}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GateAudioProcessorEditor)
};
