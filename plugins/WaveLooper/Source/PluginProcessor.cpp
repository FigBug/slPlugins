#include "PluginProcessor.h"
#include "PluginEditor.h"

WaveLooperAudioProcessor::WaveLooperAudioProcessor()
    : gin::Processor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    samplePlayer.setLooping (true);
}

WaveLooperAudioProcessor::~WaveLooperAudioProcessor()
{
}

void WaveLooperAudioProcessor::stateUpdated()
{
    auto path = state.getProperty ("samplePath", "").toString();
    if (path.isNotEmpty())
    {
        juce::File file (path);
        if (file.existsAsFile())
            samplePlayer.load (file);
    }

    samplePlayer.setLooping (state.getProperty ("loop", true));
}

void WaveLooperAudioProcessor::updateState()
{
    state.setProperty ("samplePath", samplePlayer.getLoadedFile().getFullPathName(), nullptr);
    state.setProperty ("loop", samplePlayer.isLooping(), nullptr);
}

void WaveLooperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    samplePlayer.setPlaybackSampleRate (sampleRate);
}

void WaveLooperAudioProcessor::releaseResources()
{
}

void WaveLooperAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    buffer.clear();
    samplePlayer.processBlock (buffer);
}

bool WaveLooperAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* WaveLooperAudioProcessor::createEditor()
{
    return new WaveLooperAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveLooperAudioProcessor();
}
