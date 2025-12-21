#pragma once

#include <JuceHeader.h>
#include "AudioRecorder.h"

#if __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wimplicit-float-conversion"
 #pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
 #pragma clang diagnostic ignored "-Wfloat-conversion"
 #pragma clang diagnostic ignored "-Wshift-sign-overflow"
 #pragma clang diagnostic ignored "-Wshadow-field-in-constructor"
 #pragma clang diagnostic ignored "-Wshadow"
 #pragma clang diagnostic ignored "-Wc++98-compat-extra-semi"
 #pragma clang diagnostic ignored "-Wsign-conversion"
 #pragma clang diagnostic ignored "-Wextra-semi"
 #pragma clang diagnostic ignored "-Wshorten-64-to-32"
 #pragma clang diagnostic ignored "-Wsign-compare"
 #pragma clang diagnostic ignored "-Wimplicit-int-conversion"
 #pragma clang diagnostic ignored "-Wunused-parameter"
 #pragma clang diagnostic ignored "-Wreorder-ctor"
#endif

#ifdef _MSC_VER
 #pragma warning (push)
 #pragma warning (disable: 4305 4244 4267 4100 4456)
#endif

#include <q/support/literals.hpp>
#include <q/fx/signal_conditioner.hpp>
#include <q/pitch/pitch_detector.hpp>

#if __clang__
 #pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
 #pragma warning (pop)
#endif

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
    gin::Parameter::Ptr triggerLevel;
    gin::Parameter::Ptr triggerPos;
    gin::Parameter::Ptr sync;
    gin::Parameter::Ptr beatSync;

    // Playhead info for beat sync
    std::atomic<double> lastPpqPosition { 0.0 };
    std::atomic<double> lastBpm { 120.0 };
    std::atomic<bool> lastIsPlaying { false };

    // Trace colors (stored in state)
    juce::Colour getTraceColour (int channel) const;
    void setTraceColour (int channel, juce::Colour colour);
    std::function<void()> onTraceColourChanged;

    // Pitch tracking
    float getDetectedPitch() const { return detectedPitch.load(); }

private:
    // Pitch detection
    static constexpr cycfi::q::frequency low_e { 80.0 };
    static constexpr cycfi::q::frequency high_e { 1000.0 };

    std::unique_ptr<cycfi::q::signal_conditioner> pitchConditioner;
    std::unique_ptr<cycfi::q::pitch_detector> pitchDetector;
    std::atomic<float> detectedPitch { 0.0f };
    std::atomic<float> lastDetectedPitch { 0.0f };
    int64_t samplesSinceLastPitchUpdate { 0 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
