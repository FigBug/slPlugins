#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/** Three vertical bars (left and right true peak over RMS, then momentary
    loudness) with a column of readouts for the EBU R128 measurements.
*/
class LoudnessDisplay : public juce::Component,
                        private juce::Timer
{
public:
    LoudnessDisplay (PluginProcessor&);
    ~LoudnessDisplay() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;

    std::function<void()> onContextMenu;

private:
    void timerCallback() override;

    struct ScaleInfo
    {
        float bottom, top, step;      // displayed dB range and tick spacing
        float offset;                 // added to dBFS / dBTP values
        float rmsOffset;              // extra offset for RMS (AES-17 +3 dB on K scales)
        float yellow, red;            // zone starts, displayed units
        juce::String name;
    };
    ScaleInfo scaleInfo() const;

    static juce::String formatDb (float v, const juce::String& unit, int decimals = 1);
    static float valueToY (float v, float bottom, float top, juce::Rectangle<float> bar);

    void drawLevelBar (juce::Graphics& g, juce::Rectangle<float> bar, const ScaleInfo& s, float rmsDb, float peakDb, float holdDb);
    void drawLoudnessBar (juce::Graphics& g, juce::Rectangle<float> bar, float momentary, float shortTerm);
    void drawScale (juce::Graphics& g, juce::Rectangle<float> bar, float bottom, float top, float step, bool labelsOnLeft);

    PluginProcessor& proc;

    // Displayed values. Peaks fall at a fixed rate so the bar is readable.
    float rmsDb[2]   = { -100.0f, -100.0f };
    float peakDb[2]  = { -100.0f, -100.0f };
    float holdDb[2]  = { -100.0f, -100.0f };
    float momentary = -100.0f, shortTerm = -100.0f, integrated = -100.0f, range = 0.0f, maxPeak = -100.0f;
    double elapsed = 0.0;
    int slowTick = 0;

    juce::TextButton resetButton { "Reset" };
    juce::Rectangle<int> barsArea, readoutArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoudnessDisplay)
};
