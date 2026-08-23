
#include "PluginProcessor.h"
#include <mutex>
#include "PluginEditor.h"
#include <random>

//==============================================================================
static std::variant<float, juce::String> chanTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
    {
        switch (int (*v))
        {
            case -1: return juce::String ("Ave");
            case 0:  return juce::String ("Left");
            case 1:  return juce::String ("Right");
            default: return juce::String();
        }
    }

    auto t = std::get<juce::String> (in).trim();
    if (t.equalsIgnoreCase ("Ave"))   return -1.0f;
    if (t.equalsIgnoreCase ("Left"))  return 0.0f;
    if (t.equalsIgnoreCase ("Right")) return 1.0f;
    return t.getFloatValue();
}

static std::variant<float, juce::String> modeTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    const juce::StringArray names { "Off", "Up", "Down", "Auto" };

    if (auto v = std::get_if<float> (&in))
    {
        auto idx = int (*v);
        return juce::isPositiveAndBelow (idx, names.size()) ? names[idx] : juce::String();
    }

    auto t = std::get<juce::String> (in).trim();
    for (int i = 0; i < names.size(); i++)
        if (t.equalsIgnoreCase (names[i]))
            return float (i);
    return t.getFloatValue();
}

static std::variant<float, juce::String> sppTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
    {
        if (*v < 1.0f)
            return "1/" + juce::String (juce::roundToInt (1.0f / *v));
        return juce::String (juce::roundToInt (*v));
    }

    auto t = std::get<juce::String> (in).trim();
    if (t.startsWith ("1/"))
    {
        auto denom = t.substring (2).getFloatValue();
        return denom > 0.0f ? 1.0f / denom : 1.0f;
    }
    return t.getFloatValue();
}

static std::variant<float, juce::String> tlTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
    {
        if (std::abs (*v) < 0.0001f)
            return juce::String ("-inf dB");
        return juce::String (juce::Decibels::gainToDecibels (std::abs (*v)), 1) + " dB";
    }

    auto t = std::get<juce::String> (in).trim();
    if (t.startsWithIgnoreCase ("-inf"))
        return 0.0f;
    return juce::Decibels::decibelsToGain (t.getFloatValue());
}

static std::variant<float, juce::String> tpTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
        return juce::String (*v, 2);

    return std::get<juce::String> (in).getFloatValue();
}

static std::variant<float, juce::String> runTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
    {
        switch (int (*v))
        {
            case 0:  return juce::String ("Normal");
            case 1:  return juce::String ("Single");
            default: return juce::String();
        }
    }

    auto t = std::get<juce::String> (in).trim();
    if (t.equalsIgnoreCase ("Normal")) return 0.0f;
    if (t.equalsIgnoreCase ("Single")) return 1.0f;
    return t.getFloatValue();
}

static std::variant<float, juce::String> beatSyncTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
    {
        int beats = int (*v);
        return juce::String (beats) + (beats == 1 ? " beat" : " beats");
    }

    return float (int (std::get<juce::String> (in).getFloatValue()));
}

static std::variant<float, juce::String> syncTextFunction (const gin::Parameter&, const std::variant<float, juce::String>& in)
{
    if (auto v = std::get_if<float> (&in))
        return juce::String (*v > 0.5f ? "On" : "Off");

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

PluginProcessor::PluginProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    launchCrashReporterOnce();

    fifo.setSize (2, 44100);

    samplesPerPixel  = addExtParam ("samplesPerPixel", "Samp/px",       "", "", {0.0625f, 48.0f, 0.0f, 0.3f}, 1.0f, 0.0f, sppTextFunction);
    verticalZoom     = addExtParam ("zoom",            "Zoom",          "", "", {0.1f,   100.0f, 0.0f, 0.3f}, 1.0f, 0.0f);
    verticalOffsetL  = addExtParam ("offset_l",        "Offset L",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    verticalOffsetR  = addExtParam ("offset_r",        "Offset R",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    triggerChannel   = addExtParam ("trigger_chan",    "Trigger Chan",  "", "", {-1.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, chanTextFunction);
    triggerMode      = addExtParam ("trigger_mode",    "Trigger Mode",  "", "", {0.0f,   3.0f,   1.0f, 1.0f}, 3.0f, 0.0f, modeTextFunction);
    triggerLevel     = addExtParam ("trigger_level",   "Trigger Level", "", "", {-1.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tlTextFunction);
    triggerPos       = addExtParam ("trigger_pos",     "Trigger Pos",   "", "", { 0.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tpTextFunction);
    triggerRun       = addExtParam ("trigger_run",     "Trigger Run",   "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, runTextFunction);
    sync             = addExtParam ("sync",            "Sync",          "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, syncTextFunction);
    beatSync         = addExtParam ("beat_sync",       "Beats",         "", "", { 1.0f,  32.0f,  1.0f, 1.0f}, 4.0f, 0.0f, beatSyncTextFunction);

    init();
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::numChannelsChanged()
{
    fifo.setSize (getTotalNumInputChannels(), 44100);
    recordFifo.setSize (getTotalNumInputChannels(), int (getSampleRate()));
}

void PluginProcessor::prepareToPlay (double sampleRate, int)
{
    recordFifo.setSize (getTotalNumInputChannels(), int (sampleRate));
    audioRecorder.setSampleRate (sampleRate);

    // Initialize pitch detection
    detectedPitch.store (0.0f);

    auto cfg = cycfi::q::signal_conditioner::config();
    pitchConditioner = std::make_unique<cycfi::q::signal_conditioner> (cfg, low_e, high_e, std::uint32_t (sampleRate));
    pitchDetector = std::make_unique<cycfi::q::pitch_detector> (low_e, high_e, std::uint32_t (sampleRate), cycfi::q::decibel { -45.0, cycfi::q::direct_unit });
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    // Capture playhead info for beat sync
    if (auto* playHead = getPlayHead())
    {
        if (auto posInfo = playHead->getPosition())
        {
            if (auto ppq = posInfo->getPpqPosition())
                lastPpqPosition.store (*ppq);
            if (auto bpm = posInfo->getBpm())
                lastBpm.store (*bpm);
            lastIsPlaying.store (posInfo->getIsPlaying());
        }
    }

    // Pitch detection
    if (pitchDetector && pitchConditioner)
    {
        auto& d = *pitchDetector;
        auto& c = *pitchConditioner;

        auto updatePitch = [&] (float freq)
        {
            lastDetectedPitch.store (freq);
            samplesSinceLastPitchUpdate = 0;
            detectedPitch.store (freq);
        };

        if (buffer.getNumChannels() == 1)
        {
            auto p = buffer.getReadPointer (0);
            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                auto v = c (p[i]);
                if (d (v))
                    updatePitch (float (pitchDetector->get_frequency()));
            }
        }
        else if (buffer.getNumChannels() >= 2)
        {
            auto l = buffer.getReadPointer (0);
            auto r = buffer.getReadPointer (1);

            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                auto v = (l[i] + r[i]) / 2.0f;
                v = c (v);
                if (d (v))
                    updatePitch (float (pitchDetector->get_frequency()));
            }
        }

        // Clear pitch if no update for 1 second worth of samples
        samplesSinceLastPitchUpdate += buffer.getNumSamples();
        if (samplesSinceLastPitchUpdate > int64_t (getSampleRate()))
            detectedPitch.store (0.0f);
    }

    if (fifo.getFreeSpace() >= buffer.getNumSamples())
    {
        const auto numChannels = buffer.getNumChannels();
        if (numChannels == 2)
        {
            fifo.write (buffer);
        }
        else
        {
            const auto numSamples = buffer.getNumSamples();
            gin::ScratchBuffer stereoBuffer (2, numSamples);
            stereoBuffer.clear();

            stereoBuffer.copyFrom (0, 0, buffer, 0, 0, numSamples);
            fifo.write (stereoBuffer);
        }
    }

    if (recordFifo.getFreeSpace() >= buffer.getNumSamples())
        recordFifo.write (buffer);
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
juce::Colour PluginProcessor::getTraceColour (int channel) const
{
    auto propName = "traceColour" + juce::String (channel);
    if (state.hasProperty (propName))
        return juce::Colour::fromString (state.getProperty (propName).toString());

    // Default colors
    if (channel == 0)
        return juce::Colours::white.overlaidWith (juce::Colours::blue.withAlpha (0.3f));
    else
        return juce::Colours::white.overlaidWith (juce::Colours::yellow.withAlpha (0.3f));
}

void PluginProcessor::setTraceColour (int channel, juce::Colour colour)
{
    auto propName = "traceColour" + juce::String (channel);
    state.setProperty (propName, colour.toString(), nullptr);

    if (onTraceColourChanged)
        onTraceColourChanged();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

