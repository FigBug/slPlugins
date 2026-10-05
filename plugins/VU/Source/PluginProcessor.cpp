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
void PluginProcessor::setReferenceDb (float db)
{
    referenceDb.store (db, std::memory_order_relaxed);
    state.setProperty ("reference", db, nullptr);
}

void PluginProcessor::stateUpdated()
{
    referenceDb.store (float (state.getProperty ("reference", defaultReferenceDb)), std::memory_order_relaxed);
}

void PluginProcessor::updateState()
{
    state.setProperty ("reference", getReferenceDb(), nullptr);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int)
{
    for (auto& b : ballistics)
        b.prepare (sampleRate);

    for (auto& l : level)
        l.store (0.0f, std::memory_order_relaxed);
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    if (midiLearn)
        midiLearn->processBlock (midi, numSamples);

    if (numChannels == 0 || numSamples == 0)
        return;

    // A sine of amplitude refAmp is 0 VU. Its full wave rectified average is
    // 2 * refAmp / pi, so scale by pi / (2 * refAmp) to make that read as 1.0.
    const float refAmp  = juce::Decibels::decibelsToGain (getReferenceDb());
    const float scale   = juce::MathConstants<float>::pi / (2.0f * refAmp);
    const float peakAmp = juce::Decibels::decibelsToGain (peakDb);

    for (int m = 0; m < 2; m++)
    {
        // Mono input drives both meters
        const float* src = buffer.getReadPointer (juce::jmin (m, numChannels - 1));
        auto& b = ballistics[m];
        bool hitPeak = false;

        for (int i = 0; i < numSamples; i++)
        {
            const float a = std::abs (src[i]);
            hitPeak = hitPeak || (a >= peakAmp);
            b.process (a * scale);
        }

        level[m].store (b.get(), std::memory_order_relaxed);

        if (hitPeak)
            peak[m].store (true, std::memory_order_relaxed);
    }
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
