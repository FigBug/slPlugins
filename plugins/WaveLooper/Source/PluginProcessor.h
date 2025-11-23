#pragma once

#include <JuceHeader.h>

class WaveLooperAudioProcessorEditor;

class WaveLooperAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    WaveLooperAudioProcessor();
    ~WaveLooperAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    void setFile (const juce::File& f);

private:
    void stateUpdated() override;
    void updateState() override;

    //==============================================================================
    std::unique_ptr<gin::Sample> sample;
    gin::SampleOscillator oscillator;
    
    bool playing = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessor)
};
