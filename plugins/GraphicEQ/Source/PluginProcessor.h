#pragma once

#include <JuceHeader.h>

//==============================================================================
class GraphicEQAudioProcessor : public gin::Processor
{
public:
    static constexpr int numBands = 10;

    //==============================================================================
    GraphicEQAudioProcessor();
    ~GraphicEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    // Band frequencies for 10-band EQ: 31.5, 63, 125, 250, 500, 1k, 2k, 4k, 8k, 16k Hz
    std::array<gin::Parameter::Ptr, numBands> bandGain;

    gin::Parameter::Ptr lpEnable, lpFreq, lpSlope;
    gin::Parameter::Ptr hpEnable, hpFreq, hpSlope;
    gin::Parameter::Ptr outputGain;

    gin::GraphicEQ eq { gin::GraphicEQ::Bands10 };
    gin::Filter lpFilter, hpFilter;

    gin::AudioFifo fifo;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicEQAudioProcessor)
};
