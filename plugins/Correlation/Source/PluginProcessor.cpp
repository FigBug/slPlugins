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
double PluginProcessor::windowForSpeed (int s)
{
    switch (s)
    {
        case fast:  return 0.1;
        case slow:  return 1.0;
        default:    return 0.3;
    }
}

void PluginProcessor::setSpeed (int s)
{
    speed.store (s, std::memory_order_relaxed);
    meter.setWindow (windowForSpeed (s));
    state.setProperty ("speed", s, nullptr);
}

void PluginProcessor::stateUpdated()
{
    setSpeed (juce::jlimit (0, 2, int (state.getProperty ("speed", int (medium)))));
}

void PluginProcessor::updateState()
{
    state.setProperty ("speed", getSpeed(), nullptr);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int)
{
    meter.prepare (sampleRate, windowForSpeed (getSpeed()));
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    meter.process (buffer);
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
