#pragma once

#include <JuceHeader.h>

class PluginEditor;

//==============================================================================
/** Second order VU meter ballistics, run per sample on the rectified signal.

    Tuned to the ANSI C16.5 / IEC 60268-17 response: a 0 VU step reaches 99%
    of its final value in 300 ms with roughly 1% overshoot. The input is
    expected to be pre-scaled so that the average of a 0 VU sine wave is 1.0,
    so the output is a linear "VU" deflection where 1.0 sits on the 0 mark.
*/
class VUBallistics
{
public:
    void prepare (double sampleRate)
    {
        dt = float (1.0 / sampleRate);
        reset();
    }

    void reset()
    {
        y = 0.0f;
        v = 0.0f;
    }

    inline void process (float rectified) noexcept
    {
        v += (w2 * (rectified - y) - twoZetaW * v) * dt;
        y += v * dt;
    }

    float get() const noexcept      { return y; }

private:
    static constexpr float omega     = 14.1f;   // rad/s
    static constexpr float zeta      = 0.83f;
    static constexpr float w2        = omega * omega;
    static constexpr float twoZetaW  = 2.0f * zeta * omega;

    float dt = 1.0f / 44100.0f;
    float y = 0.0f, v = 0.0f;
};

//==============================================================================
/**
*/
class PluginProcessor : public gin::Processor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    static constexpr float defaultReferenceDb = -18.0f;
    static constexpr float peakDb             =   0.0f;  // the peak lamp lights at this dBFS

    /** dBFS of a sine that reads 0 VU. Stored in the plugin state. */
    float getReferenceDb() const        { return referenceDb.load (std::memory_order_relaxed); }
    void  setReferenceDb (float db);

    void stateUpdated() override;
    void updateState() override;

    // Read by the editor. Linear VU deflection (1.0 == 0 VU) per channel, and a
    // latch that is set when a sample exceeds the peak level; the editor clears it.
    std::atomic<float> level[2] { 0.0f, 0.0f };
    std::atomic<bool>  peak[2]  { false, false };

private:
    //==============================================================================
    std::atomic<float> referenceDb { defaultReferenceDb };
    VUBallistics ballistics[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
