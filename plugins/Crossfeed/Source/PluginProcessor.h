#pragma once

#include <JuceHeader.h>
#include "../../../3rdparty/crossfeed/crossfeed.h"

//==============================================================================
/**
*/
class CrossfeedAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    CrossfeedAudioProcessor();
    ~CrossfeedAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    
    gin::LevelTracker& getOutputLevel() { return outputLevel; }
    
    gin::Parameter::Ptr enable;
    gin::AudioFifo fifo { 2, 44100 };

private:
    gin::LevelTracker outputLevel {48.0};
    juce::LinearSmoothedValue<float> enableVal;
    juce::LinearSmoothedValue<float> disableVal;

    juce::AudioSampleBuffer scratch;

    crossfeed_t crossfeed;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfeedAudioProcessor)
};

