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
    /** Averaging time, stored in the plugin state. */
    enum Speed { fast = 0, medium, slow };

    int  getSpeed() const               { return speed.load (std::memory_order_relaxed); }
    void setSpeed (int s);
    static double windowForSpeed (int s);

    void stateUpdated() override;
    void updateState() override;

    gin::CorrelationMeter meter;

private:
    //==============================================================================
    std::atomic<int> speed { medium };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
