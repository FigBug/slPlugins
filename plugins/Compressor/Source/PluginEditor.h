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
class CompressorAudioProcessorEditor : public gin::ProcessorEditor,
                                       private gin::Parameter::ParameterListener
{
public:
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void valueUpdated (gin::Parameter* param) override;

private:
    CompressorAudioProcessor& proc;
    
    gin::DynamicsMeter meter {proc.compressor};
    gin::TriggeredScope scope {proc.fifo};
    
    gin::LevelMeter inputMeter {proc.compressor.getInputTracker()};
    gin::LevelMeter outputMeter {proc.compressor.getOutputTracker()};
    gin::LevelMeter reductionMeter {proc.compressor.getReductionTracker(), {-30, 0}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
