#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiLooperAudioProcessor::MidiLooperAudioProcessor()
    : gin::Processor (BusesProperties(), true)
{
    midiPlayer.setLooping (true);
}

MidiLooperAudioProcessor::~MidiLooperAudioProcessor()
{
}

void MidiLooperAudioProcessor::stateUpdated()
{
    auto path = state.getProperty ("midiPath", "").toString();
    if (path.isNotEmpty())
    {
        juce::File file (path);
        if (file.existsAsFile())
            midiPlayer.load (file);
    }

    midiPlayer.setLooping (state.getProperty ("loop", true));
}

void MidiLooperAudioProcessor::updateState()
{
    state.setProperty ("midiPath", midiPlayer.getLoadedFile().getFullPathName(), nullptr);
    state.setProperty ("loop", midiPlayer.isLooping(), nullptr);
}

void MidiLooperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    midiPlayer.setSampleRate (sampleRate);
}

void MidiLooperAudioProcessor::releaseResources()
{
}

void MidiLooperAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();
    midi.clear();
    midiPlayer.processBlock (buffer.getNumSamples(), midi);
}

bool MidiLooperAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MidiLooperAudioProcessor::createEditor()
{
    return new MidiLooperAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiLooperAudioProcessor();
}
