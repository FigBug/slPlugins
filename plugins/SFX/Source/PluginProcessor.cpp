/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"

using namespace gin;

//==============================================================================
SFXAudioProcessor::SFXAudioProcessor()
{
    // Add voices
    for (int i = 0; i < 32; i++)
        addVoice (new Voice (*this));

    int notes[] = { 64, 65, 66, 67,
                    60, 61, 62, 63,
                    56, 57, 58, 59,
                    52, 53, 54, 55 };

    // Add pads
    for (int i = 0; i < 16; i++)
        pads.add (new Pad (notes[i]));
    
    // Add parameters
    for (int i = 0; i < 16; i++)
    {
        SfxrParams p;
        auto ids = p.getParams();
        for (auto id : ids)
        {
            String uniqueId = String (id.c_str()) + String (i + 1);

            addPluginParameter (new Parameter (uniqueId,
                                               String (p.getName (id).c_str()) + " " + String (i + 1),
                                               p.getName (id),
                                               "",
                                               p.getMin (id),
                                               p.getMax (id),
                                               0.0f,
                                               p.getDefault (id)));
        }
    }

    // Add parameters to pads
    auto allParams = getPluginParameters();
    int paramsPerPad = allParams.size() / 16;

    for (int i = 0; i < 16; i++)
    {
        auto p = pads[i];

        for (int j = 0; j < paramsPerPad; j++)
            p->pluginParams.add (allParams[i * paramsPerPad + j]);

        p->toPluginParams();
    }
}

SFXAudioProcessor::~SFXAudioProcessor()
{
}

//==============================================================================
void SFXAudioProcessor::stateUpdated()
{
}

void SFXAudioProcessor::updateState()
{
}

//==============================================================================
void SFXAudioProcessor::midiNoteOn (int note, int velocity)
{
    ScopedLock sl (lock);

    userMidi.addEvent (MidiMessage::noteOn (1, note, uint8 (velocity)), 0);
}

void SFXAudioProcessor::midiNoteOff (int note, int velocity)
{
    ScopedLock sl (lock);

    userMidi.addEvent (MidiMessage::noteOff (1, note, uint8 (velocity)), 0);
}

//==============================================================================
void SFXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setCurrentPlaybackSampleRate (sampleRate);
}

void SFXAudioProcessor::releaseResources()
{
}

void SFXAudioProcessor::trackMidi (MidiBuffer& midi, int numSamples)
{
    // fade out old messages
    double t = numSamples / gin::GinProcessor::getSampleRate() * 1000;
    for (auto& c : midiCnt)
        c = jmax (0, int (c - t));

    // track new messages
    int pos = 0;
    MidiMessage msg;
    auto itr = MidiBuffer::Iterator (midi);

    while (itr.getNextEvent (msg, pos))
    {
        int n = msg.getNoteNumber();
        if (msg.isNoteOn())
        {
            midiOn[n]++;
            midiCnt[n] = 100;
        }
        else if (msg.isNoteOff())
        {
            midiOn[n]--;
            if (midiOn[n] < 0)
                midiOn[n] = 0;
        }
        else if (msg.isAllNotesOff() || msg.isAllSoundOff())
        {
            memset (midiOn, 0, sizeof (midiOn));
            memset (midiCnt, 0, sizeof (midiCnt));
        }
    }
}

void SFXAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midi)
{
    ScopedLock sl (lock);
    ScopedNoDenormals noDenormals;

    if (userMidi.getNumEvents() > 0)
    {
        userMidi.addEvents (midi, 0, buffer.getNumSamples(), 0);

        trackMidi (userMidi, buffer.getNumSamples());
        renderNextBlock (buffer, userMidi, 0, buffer.getNumSamples());
        userMidi.clear();
    }
    else
    {
        trackMidi (midi, buffer.getNumSamples());
        renderNextBlock (buffer, midi, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SFXAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SFXAudioProcessor::createEditor()
{
    editor = new SFXAudioProcessorEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SFXAudioProcessor();
}
