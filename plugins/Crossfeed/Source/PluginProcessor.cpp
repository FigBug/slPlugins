
#include "PluginProcessor.h"
#include <mutex>
#include <cstdlib>
#include "PluginEditor.h"

static std::variant<float, juce::String> enableTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
        return juce::String (*v > 0.0f ? "On" : "Off");

    auto t = std::get<juce::String> (in).trim();
    if (t.equalsIgnoreCase ("On"))  return 1.0f;
    if (t.equalsIgnoreCase ("Off")) return 0.0f;
    return t.getFloatValue();
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

    // in case the host processes before prepareToPlay, don't leave the filter
    // uninitialized (divide by zero / garbage kernel in processBlock)
    crossfeed_init (&crossfeed, 44100);

    enable = addExtParam ("enable",    "Enable", "", "",    { 0.0f,   1.0f, 1.0f, 1.0f}, 1.0f, 0.0f, enableTextFunction);

    init();
}

CrossfeedAudioProcessor::~CrossfeedAudioProcessor()
{
}

//==============================================================================
void CrossfeedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // crossfeed only has kernels for 44.1k, 48k and 96k; snap to the nearest
    // so init can't fail and leave a zeroed filter (divide by zero in processBlock)
    int bestRate = 44100;
    for (int rate : { 48000, 96000 })
        if (std::abs ((int)sampleRate - rate) < std::abs ((int)sampleRate - bestRate))
            bestRate = rate;

    crossfeed_init (&crossfeed, bestRate);
    
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

