
#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"

static juce::String enableTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
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

CrossfeedAudioProcessor::CrossfeedAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    launchCrashReporterOnce();

    enable = addExtParam ("enable",    "Enable", "", "",    { 0.0f,   1.0f, 1.0f, 1.0f}, 1.0f, 0.0f, enableTextFunction);

    init();
}

CrossfeedAudioProcessor::~CrossfeedAudioProcessor()
{
}

//==============================================================================
void CrossfeedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    crossfeed_init (&crossfeed, (int)sampleRate);
    
    scratch.setSize (2, samplesPerBlock);
    
    enableVal.reset (sampleRate, 0.05);
    disableVal.reset (sampleRate, 0.05);
}

void CrossfeedAudioProcessor::releaseResources()
{
}

void CrossfeedAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    const auto numSamples = buffer.getNumSamples();

    if (midiLearn)
        midiLearn->processBlock (midi, numSamples);

    auto pre = gin::monoBuffer (buffer);

    enableVal.setTargetValue (enable->getUserValue() > 0.5f ? 1.0f : 0.0f);
    disableVal.setTargetValue (enable->getUserValue() > 0.5f ? 0.0f : 1.0f);
    
    scratch.makeCopyOf (buffer, true);
    
    crossfeed_filter_inplace_noninterleaved (&crossfeed, scratch.getWritePointer (0), scratch.getWritePointer (1), (unsigned int)numSamples);

    auto post = gin::monoBuffer (scratch);

    gin::ScratchBuffer analyze (2, numSamples);
    analyze.clear();
    analyze.addFrom (0, 0, pre, 0, 0, numSamples);
    analyze.addFrom (1, 0, post, 0, 0, numSamples);
    if (fifo.getFreeSpace() >= numSamples)
        fifo.write (analyze);

    gin::applyGain (buffer, disableVal);
    gin::applyGain (scratch, enableVal);
    
    buffer.addFrom (0, 0, scratch, 0, 0, numSamples);
    buffer.addFrom (1, 0, scratch, 1, 0, numSamples);
}

//==============================================================================
bool CrossfeedAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CrossfeedAudioProcessor::createEditor()
{
    return new CrossfeedAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrossfeedAudioProcessor();
}

