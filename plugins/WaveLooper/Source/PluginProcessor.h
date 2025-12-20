#pragma once

#include <JuceHeader.h>

class WaveLooperAudioProcessorEditor;

class WaveLooperAudioProcessor : public gin::Processor
{
public:
    WaveLooperAudioProcessor();
    ~WaveLooperAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    gin::SamplePlayer samplePlayer;

private:
    void stateUpdated() override;
    void updateState() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessor)
};
