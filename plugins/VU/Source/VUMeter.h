#pragma once

#include <JuceHeader.h>

//==============================================================================
/** A flat, minimal VU meter: arc scale with the standard -20 to +3 marks,
    a needle whose pivot sits below the frame, and a peak dot.
*/
class VUMeter : public juce::Component,
                private juce::Timer
{
public:
    VUMeter (std::atomic<float>& level, std::atomic<bool>& peak, const juce::String& name);
    ~VUMeter() override;

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent&) override;

    /** Small caption drawn along the bottom edge, e.g. the 0 VU reference. */
    void setCaption (const juce::String&);

    std::function<void()> onContextMenu;

    static constexpr float faceAspect = 2.4f;

private:
    void timerCallback() override;

    static float linearToPosition (float linear)   { return (linear - minLinear) / (maxLinear - minLinear); }
    static float positionToAngle (float pos)       { return juce::degreesToRadians (sweepDegrees * (2.0f * pos - 1.0f)); }

    std::atomic<float>& levelSource;
    std::atomic<bool>&  peakSource;
    juce::String channelName, caption;

    float needleLinear = 0.0f;
    juce::uint32 peakLitUntil = 0;
    bool peakLit = false;

    static constexpr float sweepDegrees = 45.0f;     // half sweep either side of vertical
    static constexpr float minLinear    = 0.1f;      // -20 VU
    static constexpr float maxLinear    = 1.41254f;  //  +3 VU

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VUMeter)
};
