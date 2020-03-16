#pragma once

#include <JuceHeader.h>

class WaveLooperAudioProcessorEditor;

class WaveLooperAudioProcessor : public gin::GinProcessor
{
public:
    //==============================================================================
    WaveLooperAudioProcessor();
    ~WaveLooperAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

private:
    //==============================================================================

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessor)
};
