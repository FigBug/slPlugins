#pragma once

#include <JuceHeader.h>

//==============================================================================
/** Horizontal correlation bar from -1 to +1 with a stereo balance indicator
    beneath it. Flat, minimal, drawn in the plugin's look and feel colours.
*/
class CorrelationDisplay : public juce::Component,
                           private juce::Timer
{
public:
    CorrelationDisplay (const gin::CorrelationMeter& meter);
    ~CorrelationDisplay() override;

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent&) override;

    std::function<void()> onContextMenu;

private:
    void timerCallback() override;

    const gin::CorrelationMeter& meter;
    float correlation = 0.0f, balance = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CorrelationDisplay)
};
