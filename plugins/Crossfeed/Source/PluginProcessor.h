#pragma once

#include <JuceHeader.h>
#include "../../../3rdparty/crossfeed/crossfeed.h"

#define PARAM_ENABLE        "enable"

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

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    
    gin::LevelTracker& getOutputLevel() { return outputLevel; }
    
    gin::Parameter::Ptr enable;

private:
    gin::LevelTracker outputLevel {48.0};
    LinearSmoothedValue<float> enableVal;
    LinearSmoothedValue<float> disableVal;
    
    AudioSampleBuffer scratch;
    
    crossfeed_t crossfeed;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossfeedAudioProcessor)
};

