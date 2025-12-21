
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
static juce::String chanTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case -1: return "Ave";
        case 0:  return "Left";
        case 1:  return "Right";
        default: return "";
    }
}

static juce::String modeTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Off";
        case 1:  return "Up";
        case 2:  return "Down";
        case 3:  return "Auto";
        default: return "";
    }
}

static juce::String sppTextFunction (const gin::Parameter&, float v)
{
    if (v < 1.0f)
        return "1/" + juce::String (juce::roundToInt (1.0f / v));
    return juce::String (juce::roundToInt (v));
}

static juce::String tlTextFunction (const gin::Parameter&, float v)
{
    if (std::abs (v) < 0.0001f)
        return "-inf dB";
    return juce::String (juce::Decibels::gainToDecibels (std::abs (v)), 1) + " dB";
}

static juce::String tpTextFunction (const gin::Parameter&, float v)
{
    return juce::String (v, 2);
}

static juce::String runTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Normal";
        case 1:  return "Single";
        default: return "";
    }
}

static juce::String beatSyncTextFunction (const gin::Parameter&, float v)
{
    int beats = int (v);
    return juce::String (beats) + (beats == 1 ? " beat" : " beats");
}

static juce::String syncTextFunction (const gin::Parameter&, float v)
{
    return v > 0.5f ? "On" : "Off";
}

//==============================================================================
PluginProcessor::PluginProcessor()
{
    fifo.setSize (2, 44100);

    samplesPerPixel  = addExtParam ("samplesPerPixel", "Samp/px",       "", "", {0.0625f, 48.0f, 0.0f, 0.3f}, 1.0f, 0.0f, sppTextFunction);
    verticalZoom     = addExtParam ("zoom",            "Zoom",          "", "", {0.1f,   100.0f, 0.0f, 0.3f}, 1.0f, 0.0f);
    verticalOffsetL  = addExtParam ("offset_l",        "Offset L",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    verticalOffsetR  = addExtParam ("offset_r",        "Offset R",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    triggerChannel   = addExtParam ("trigger_chan",    "Trigger Chan",  "", "", {-1.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, chanTextFunction);
    triggerMode      = addExtParam ("trigger_mode",    "Trigger Mode",  "", "", {0.0f,   3.0f,   1.0f, 1.0f}, 1.0f, 0.0f, modeTextFunction);
    triggerLevel     = addExtParam ("trigger_level",   "Trigger Level", "", "", {-1.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tlTextFunction);
    triggerPos       = addExtParam ("trigger_pos",     "Trigger Pos",   "", "", { 0.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tpTextFunction);
    triggerRun       = addExtParam ("trigger_run",     "Trigger Run",   "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, runTextFunction);
    sync             = addExtParam ("sync",            "Sync",          "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, syncTextFunction);
    beatSync         = addExtParam ("beat_sync",       "Beats",         "", "", { 1.0f,  32.0f,  1.0f, 1.0f}, 4.0f, 0.0f, beatSyncTextFunction);
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

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
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

        if (buffer.getNumChannels() == 1)
        {
            auto p = buffer.getReadPointer (0);
            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                auto v = c (p[i]);
                if (d (v))
                    detectedPitch.store (float (pitchDetector->get_frequency()));
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
                    detectedPitch.store (float (pitchDetector->get_frequency()));
            }
        }
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
