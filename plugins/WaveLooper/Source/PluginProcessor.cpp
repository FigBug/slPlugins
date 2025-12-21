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
    autoPlay = state.getProperty ("autoPlay", true);
}

void WaveLooperAudioProcessor::updateState()
{
    state.setProperty ("samplePath", samplePlayer.getLoadedFile().getFullPathName(), nullptr);
    state.setProperty ("loop", samplePlayer.isLooping(), nullptr);
    state.setProperty ("autoPlay", autoPlay, nullptr);
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

    if (autoPlay && samplePlayer.hasFileLoaded())
    {
        auto* playHead = getPlayHead();
        if (playHead != nullptr)
        {
            auto posInfo = playHead->getPosition();
            if (posInfo.hasValue())
            {
                const bool isHostPlaying = posInfo->getIsPlaying();

                if (isHostPlaying && ! wasHostPlaying)
                    samplePlayer.play();
                else if (! isHostPlaying && wasHostPlaying)
                    samplePlayer.stop();

                wasHostPlaying = isHostPlaying;

                if (isHostPlaying)
                {
                    const double sampleLength = samplePlayer.getLengthInSeconds();
                    if (sampleLength > 0.0)
                    {
                        if (auto timeInSeconds = posInfo->getTimeInSeconds())
                        {
                            double hostPos = *timeInSeconds;
                            if (samplePlayer.isLooping())
                                hostPos = std::fmod (hostPos, sampleLength);

                            const double currentPos = samplePlayer.getPositionInSeconds();
                            if (std::abs (hostPos - currentPos) > 0.01)
                            {
                                const double samplePos = hostPos * samplePlayer.getSourceSampleRate();
                                samplePlayer.setPosition (samplePos);
                            }
                        }
                    }
                }
            }
        }
    }

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
