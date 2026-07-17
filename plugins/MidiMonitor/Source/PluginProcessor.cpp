#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"

static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions();
}

static juce::String onOffTextFunction (const gin::Parameter&, float v)
{
    return v > 0.5f ? "On" : "Off";
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

MidiMonitorAudioProcessor::MidiMonitorAudioProcessor()
    : gin::Processor (
       #if JucePlugin_Build_Standalone
        BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true),
       #else
        BusesProperties(),
       #endif
        false, createProcessorOptions())
{
    launchCrashReporterOnce();

    showNoteOn    = addIntParam (PARAM_SHOW_NOTE_ON,    "Show Note On",    "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showNoteOff   = addIntParam (PARAM_SHOW_NOTE_OFF,   "Show Note Off",   "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showCC        = addIntParam (PARAM_SHOW_CC,         "Show CC",         "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showPitchBend = addIntParam (PARAM_SHOW_PITCH_BEND, "Show Pitch Bend", "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showPressure  = addIntParam (PARAM_SHOW_PRESSURE,   "Show Pressure",   "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showSystem    = addIntParam (PARAM_SHOW_SYSTEM,     "Show System",     "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);
    showOther     = addIntParam (PARAM_SHOW_OTHER,      "Show Other",      "", "", { 0.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f, onOffTextFunction);

    init();
}

MidiMonitorAudioProcessor::~MidiMonitorAudioProcessor()
{
    cancelPendingUpdate();
}

void MidiMonitorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    currentSampleRate = sampleRate;
    processedSeconds  = 0.0;
}

void MidiMonitorAudioProcessor::releaseResources()
{
}

void MidiMonitorAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    keyboardState.processNextMidiBuffer (midi, 0, buffer.getNumSamples(), true);

    double startTime = processedSeconds;
    if (auto* ph = getPlayHead())
        if (auto pos = ph->getPosition())
            if (auto t = pos->getTimeInSeconds())
                startTime = *t;

    {
        const juce::ScopedLock sl (pendingLock);

        for (const auto meta : midi)
        {
            auto m = meta.getMessage();
            const double offsetSeconds = meta.samplePosition / currentSampleRate;
            m.setTimeStamp (startTime + offsetSeconds);

            pendingMidiMessageList.add ({ m, ++count });

            if (m.isNoteOn())
                noteState.addNote (m.getChannel(), m.getNoteNumber());
            else if (m.isNoteOff())
                noteState.removeNote (m.getChannel(), m.getNoteNumber());
            else if (m.isAllNotesOff() || m.isAllSoundOff())
                noteState.clearChannel (m.getChannel());
        }

        if (pendingMidiMessageList.size() > 0)
            triggerAsyncUpdate();
    }

    processedSeconds += buffer.getNumSamples() / currentSampleRate;
}

void MidiMonitorAudioProcessor::clearLog()
{
    count = 0;
    midiMessageList.clear();
    sendChangeMessage();
}

void MidiMonitorAudioProcessor::handleAsyncUpdate()
{
    juce::Array<std::pair<juce::MidiMessage, int>> list;

    {
        const juce::ScopedLock sl (pendingLock);
        pendingMidiMessageList.swapWith (list);
    }

    for (const auto& m : list)
        midiMessageList.add (m);

    while (midiMessageList.size() > 5000)
        midiMessageList.remove (0);

    sendChangeMessage();
}

bool MidiMonitorAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MidiMonitorAudioProcessor::createEditor()
{
    return new MidiMonitorAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiMonitorAudioProcessor();
}
