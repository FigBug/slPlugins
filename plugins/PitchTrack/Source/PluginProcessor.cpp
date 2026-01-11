#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr cycfi::q::frequency low_e          = cycfi::q::pitch_names::E[0];
constexpr cycfi::q::frequency high_e         = cycfi::q::pitch_names::E[7];

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

PitchTrackAudioProcessor::PitchTrackAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    init();
}

PitchTrackAudioProcessor::~PitchTrackAudioProcessor()
{
}

//==============================================================================
void PitchTrackAudioProcessor::prepareToPlay (double sampleRate_, int blockSize_)
{
    gin::Processor::prepareToPlay (sampleRate_, blockSize_);

    lastDetectedPitch.store (0.0);
    samplesSinceLastPitchUpdate = 0;
    detectedPitch.store (0.0);

    auto cfg = cycfi::q::signal_conditioner::config();
    pitchConditioner = std::make_unique<cycfi::q::signal_conditioner> (cfg, low_e, high_e, std::uint32_t (sampleRate_));
    pitchDetector = std::make_unique<cycfi::q::pitch_detector> (low_e, high_e, std::uint32_t (sampleRate_), cycfi::q::decibel { -45.0, cycfi::q::direct_unit });

    fifo.setSize (1, std::max (blockSize_ * 2, 512 * 2));
}

void PitchTrackAudioProcessor::releaseResources()
{
}

void PitchTrackAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    //
    // cycfi::q
    //
    if (pitchDetector && pitchConditioner)
    {
        auto& d = *pitchDetector;
        auto& c = *pitchConditioner;

        auto updatePitch = [&] (float freq)
        {
            lastDetectedPitch.store (freq);
            samplesSinceLastPitchUpdate = 0;
            detectedPitch.store (freq);
        };

        if (buffer.getNumChannels() == 1)
        {
            auto p = buffer.getReadPointer (0);
            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                auto v = c (p[i]);
                if (d (v))
                    updatePitch (float (pitchDetector->get_frequency()));
            }
        }
        else if (buffer.getNumChannels() >= 2)
        {
            auto l = buffer.getReadPointer (0);
            auto r = buffer.getReadPointer (1);

            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                auto v = (l[i] + r[i]) / 2.0f;
                v = c (v);
                if (d (v))
                    updatePitch (float (pitchDetector->get_frequency()));
            }
        }

        // Clear pitch if no update for 1 second worth of samples
        samplesSinceLastPitchUpdate += buffer.getNumSamples();
        if (samplesSinceLastPitchUpdate > int64_t (getSampleRate()))
            detectedPitch.store (0.0f);
    }
    DBG(detectedPitch.load());
}

float PitchTrackAudioProcessor::getPitch()
{
    return detectedPitch.load();
}

//==============================================================================
bool PitchTrackAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PitchTrackAudioProcessor::createEditor()
{
    return new PitchTrackAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchTrackAudioProcessor();
}

