#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String onOffTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

juce::String modeTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Spectroscope";
        case 1:  return "Sonogram";
        default: return "";
    }
}

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

    addExtParam (PARAM_MODE, "Mode", "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, modeTextFunction);
    addExtParam (PARAM_LOG,  "Log",  "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, onOffTextFunction);

    init();
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double, int)
{
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    juce::ScopedLock sl (lock);
    if (editor != nullptr)
    {
        const int num = buffer.getNumSamples();
        editor->scopeL.copySamples (buffer.getReadPointer (0), num);
        editor->sonogramL.copySamples (buffer.getReadPointer (0), num);
        
        if (getTotalNumInputChannels() > 1)
        {
            editor->scopeR.copySamples (buffer.getReadPointer (1), num);
            editor->sonogramR.copySamples (buffer.getReadPointer (1), num);
        }
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    editor = new PluginEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

