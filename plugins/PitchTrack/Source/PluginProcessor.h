#pragma once

#include <JuceHeader.h>

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
#include <q/support/pitch_names.hpp>
#include <q/fx/signal_conditioner.hpp>
#include <q/pitch/pitch_detector.hpp>

#if __clang__
 #pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
 #pragma warning (pop)
#endif


//==============================================================================
/**
*/
class PitchTrackAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    PitchTrackAudioProcessor();
    ~PitchTrackAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    
    gin::LevelTracker& getOutputLevel() { return outputLevel; }
    float getPitch();

private:
    gin::LevelTracker outputLevel {48.0};

    std::unique_ptr<cycfi::q::signal_conditioner> conditioner;
    std::unique_ptr<cycfi::q::pitch_detector> detector;

    std::unique_ptr<adamski::PitchYIN> yin;
    std::unique_ptr<adamski::PitchMPM> mpm;

    float freq = 0.0f;

    gin::AudioFifo fifo;
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchTrackAudioProcessor)
};
