/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace cycfi::q;
using namespace cycfi::q::literals;
using namespace cycfi::q::notes;

CONSTEXPR frequency low_e          = E[2];
CONSTEXPR frequency high_e         = E[4];

//==============================================================================
PitchTrackAudioProcessor::PitchTrackAudioProcessor()
{
}

PitchTrackAudioProcessor::~PitchTrackAudioProcessor()
{
}

//==============================================================================
void PitchTrackAudioProcessor::prepareToPlay (double sampleRate_, int)
{
    detector = std::make_unique<pitch_detector> (low_e, high_e * 6, sampleRate_, -45_dB);
}

void PitchTrackAudioProcessor::releaseResources()
{
}

void PitchTrackAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    if (buffer.getNumChannels() == 1)
    {
        auto p = buffer.getReadPointer (0);
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            auto& d = *detector;
            d (p[i]);
        }
    }
    else
    {
        auto l = buffer.getReadPointer (0);
        auto r = buffer.getReadPointer (1);
        
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            auto& d = *detector;
            d ((l[i] + r[i]) / 2);
        }
    }
}

float PitchTrackAudioProcessor::getPitch()
{
    if (detector != nullptr)
        return detector->get_frequency();
    return {};
}

//==============================================================================
bool PitchTrackAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PitchTrackAudioProcessor::createEditor()
{
    return new PitchTrackAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchTrackAudioProcessor();
}
