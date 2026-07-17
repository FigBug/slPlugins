#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String abTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "A" : "B";
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

AddInvertAudioProcessor::AddInvertAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    launchCrashReporterOnce();

    init();
}

AddInvertAudioProcessor::~AddInvertAudioProcessor()
{
}

//==============================================================================
void AddInvertAudioProcessor::prepareToPlay (double, int)
{
}

void AddInvertAudioProcessor::releaseResources()
{
}

void AddInvertAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    const int numChans = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    float* aL = (numChans >= 1) ? buffer.getWritePointer (0) : nullptr;
    float* aR = (numChans >= 2) ? buffer.getWritePointer (1) : nullptr;
    float* bL = (numChans >= 3) ? buffer.getWritePointer (2) : nullptr;
    float* bR = (numChans >= 4) ? buffer.getWritePointer (3) : nullptr;

    if (aL && bL) juce::FloatVectorOperations::addWithMultiply (aL, bL, -1.0f, numSamples);
    if (bL && bR) juce::FloatVectorOperations::addWithMultiply (aR, bR, -1.0f, numSamples);
    
    juce::AudioSampleBuffer outputBuffer (buffer.getArrayOfWritePointers(), 2, numSamples);
    if (fifo.getFreeSpace() >= numSamples)
        fifo.write (outputBuffer);
}

//==============================================================================
bool AddInvertAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AddInvertAudioProcessor::createEditor()
{
    return new AddInvertAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AddInvertAudioProcessor();
}

