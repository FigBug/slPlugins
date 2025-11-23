#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class EQ3AudioProcessorEditor : public gin::ProcessorEditor,
                                private gin::Parameter::ParameterListener
{
public:
    EQ3AudioProcessorEditor (EQ3AudioProcessor&);
    ~EQ3AudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void valueUpdated (gin::Parameter* param) override;

private:
    EQ3AudioProcessor& proc;

    gin::DynamicsMeter meter {proc.gate};
    gin::TriggeredScope scope {proc.fifo};

    gin::LevelMeter inputMeter {proc.gate.getInputTracker()};
    gin::LevelMeter outputMeter {proc.gate.getOutputTracker()};
    gin::LevelMeter reductionMeter {proc.gate.getReductionTracker(), {-30, 0}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQ3AudioProcessorEditor)
};
