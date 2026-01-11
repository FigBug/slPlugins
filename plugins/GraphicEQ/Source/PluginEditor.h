#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Filter response curve overlay - draws the combined EQ + filter response
class FilterResponseCurve : public juce::Component,
                            private juce::Timer
{
public:
    FilterResponseCurve (GraphicEQAudioProcessor& p);
    ~FilterResponseCurve() override;

    void paint (juce::Graphics& g) override;
    void timerCallback() override;

private:
    GraphicEQAudioProcessor& proc;

    float frequencyToX (float freq) const;
    float dbToY (float db) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterResponseCurve)
};

//==============================================================================
class GraphicEQAudioProcessorEditor : public gin::ProcessorEditor,
                                       private gin::Parameter::ParameterListener
{
public:
    GraphicEQAudioProcessorEditor (GraphicEQAudioProcessor&);
    ~GraphicEQAudioProcessorEditor() override;

    void resized() override;
    void paint (juce::Graphics& g) override;

private:
    void valueUpdated (gin::Parameter* param) override;
    void updateFilterEnableStates();

    GraphicEQAudioProcessor& proc;

    juce::OwnedArray<gin::Fader> bandFaders;

    gin::Switch lpEnableButton { proc.lpEnable };
    gin::Knob lpFreqKnob { proc.lpFreq };
    gin::Switch lpSlopeButton { proc.lpSlope };

    gin::Switch hpEnableButton { proc.hpEnable };
    gin::Knob hpFreqKnob { proc.hpFreq };
    gin::Switch hpSlopeButton { proc.hpSlope };

    gin::Fader outputFader { proc.outputGain };

    gin::SpectrumAnalyzer spectrum { proc.fifo };
    FilterResponseCurve responseCurve { proc };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEQAudioProcessorEditor)
};
