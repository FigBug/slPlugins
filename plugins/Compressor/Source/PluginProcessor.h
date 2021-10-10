#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class CompressorAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    void numChannelsChanged() override;
    
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::Parameter::Ptr attack, release, ratio, threshold, knee, input, output;

    gin::Dynamics compressor;
    gin::AudioFifo fifo;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};
