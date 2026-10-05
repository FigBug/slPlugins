#pragma once

#include <JuceHeader.h>

class PluginEditor;

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
    /** Scale for the level bars, stored in the plugin state. */
    enum Scale { dbfs = 0, k20, k14, k12 };

    int  getScale() const               { return scale.load (std::memory_order_relaxed); }
    void setScale (int s);

    /** Clears the integrated, range and held peak readings. */
    void resetMeters();

    void stateUpdated() override;
    void updateState() override;

    gin::LoudnessMeter loudness;
    gin::TruePeakMeter truePeak;
    gin::RMSMeter      rms;

private:
    //==============================================================================
    std::atomic<int> scale { dbfs };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
