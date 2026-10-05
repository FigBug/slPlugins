#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"

//==============================================================================
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

PluginProcessor::PluginProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    launchCrashReporterOnce();

    init();
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::setScale (int s)
{
    scale.store (s, std::memory_order_relaxed);
    state.setProperty ("scale", s, nullptr);
}

void PluginProcessor::resetMeters()
{
    loudness.reset();
    truePeak.reset();
    rms.reset();
}

void PluginProcessor::stateUpdated()
{
    scale.store (juce::jlimit (0, 3, int (state.getProperty ("scale", int (dbfs)))), std::memory_order_relaxed);
}

void PluginProcessor::updateState()
{
    state.setProperty ("scale", getScale(), nullptr);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const int channels = juce::jlimit (1, 2, getTotalNumInputChannels());

    loudness.prepare (sampleRate, channels);
    truePeak.prepare (sampleRate, channels, samplesPerBlock);
    rms.prepare (sampleRate, channels, 0.3);
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
        return;

    loudness.process (buffer);
    truePeak.process (buffer);
    rms.process (buffer);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
