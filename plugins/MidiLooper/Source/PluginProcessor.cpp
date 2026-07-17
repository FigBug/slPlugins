#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"

static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

// If the shared CrashReporter is installed, launch it once per process (on the
// first plugin instance) so it can scan and upload any crash from last session.
static void launchCrashReporterOnce()
{
    static std::once_flag flag;
    std::call_once (flag, []
    {
       #if JUCE_MAC
        juce::File app ("/Library/Application Support/Rabien Software/Crash Reporter/CrashReporter.app");
       #elif JUCE_WINDOWS
        auto app = juce::File::getSpecialLocation (juce::File::globalApplicationsDirectory)
                       .getChildFile ("Rabien Software").getChildFile ("Crash Reporter").getChildFile ("CrashReporter.exe");
       #else
        juce::File app;
       #endif

        if (app.exists())
            juce::Process::openDocument (app.getFullPathName(), {});
    });
}

MidiLooperAudioProcessor::MidiLooperAudioProcessor()
    : gin::Processor (BusesProperties(), false, createProcessorOptions())
{
    launchCrashReporterOnce();

    midiPlayer.setLooping (true);
    init();
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
    autoPlay = state.getProperty ("autoPlay", true);
}

void MidiLooperAudioProcessor::updateState()
{
    state.setProperty ("midiPath", midiPlayer.getLoadedFile().getFullPathName(), nullptr);
    state.setProperty ("loop", midiPlayer.isLooping(), nullptr);
    state.setProperty ("autoPlay", autoPlay, nullptr);
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
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    buffer.clear();
    midi.clear();

    if (autoPlay && midiPlayer.hasFileLoaded())
    {
        auto* playHead = getPlayHead();
        if (playHead != nullptr)
        {
            auto posInfo = playHead->getPosition();
            if (posInfo.hasValue())
            {
                const bool isHostPlaying = posInfo->getIsPlaying();

                if (isHostPlaying && ! wasHostPlaying)
                    midiPlayer.play();
                else if (! isHostPlaying && wasHostPlaying)
                    midiPlayer.stop();

                wasHostPlaying = isHostPlaying;

                if (isHostPlaying)
                {
                    const double midiLength = midiPlayer.getLengthInSeconds();
                    if (midiLength > 0.0)
                    {
                        if (auto timeInSeconds = posInfo->getTimeInSeconds())
                        {
                            double hostPos = *timeInSeconds;
                            if (midiPlayer.isLooping())
                                hostPos = std::fmod (hostPos, midiLength);

                            const double currentPos = midiPlayer.getPlayheadPosition();
                            if (std::abs (hostPos - currentPos) > 0.01)
                                midiPlayer.setPlayheadPosition (hostPos);
                        }
                    }
                }
            }
        }
    }

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

