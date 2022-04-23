#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LimiterAudioProcessorEditor : public gin::ProcessorEditor,
                                    private gin::Parameter::Listener
{
public:
    LimiterAudioProcessorEditor (LimiterAudioProcessor&);
    ~LimiterAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void parameterChanged (gin::Parameter* param) override;

private:
    LimiterAudioProcessor& limProc;
    
    gin::DynamicsMeter meter {limProc.limiter};
    gin::TriggeredScope scope {limProc.fifo};
    
    gin::LevelMeter inputMeter {limProc.limiter.getInputTracker()};
    gin::LevelMeter outputMeter {limProc.limiter.getOutputTracker()};
    gin::LevelMeter reductionMeter {limProc.limiter.getReductionTracker(), {-30, 0}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LimiterAudioProcessorEditor)
};
