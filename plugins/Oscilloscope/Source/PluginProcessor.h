#pragma once

#include <JuceHeader.h>
#include "AudioRecorder.h"

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
    void numChannelsChanged() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    gin::AudioFifo fifo;
    gin::AudioFifo recordFifo { 2, 44100 };
    AudioRecorder audioRecorder { recordFifo };

    gin::Parameter::Ptr samplesPerPixel;
    gin::Parameter::Ptr verticalZoom;
    gin::Parameter::Ptr verticalOffsetL;
    gin::Parameter::Ptr verticalOffsetR;
    gin::Parameter::Ptr triggerChannel;
    gin::Parameter::Ptr triggerMode;
    gin::Parameter::Ptr triggerRun;
    gin::Parameter::Ptr triggerReset;
    gin::Parameter::Ptr triggerLevel;
    gin::Parameter::Ptr triggerPos;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
