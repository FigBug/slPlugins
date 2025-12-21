
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
        default: return "";
    }
}

static juce::String intTextFunction (const gin::Parameter&, float v)
{
    return juce::String (int (v));
}

static juce::String tlTextFunction (const gin::Parameter&, float v)
{
    return juce::String (v, 2);
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

static juce::String resetTextFunction (const gin::Parameter&, float)
{
    return "Reset";
}

//==============================================================================
PluginProcessor::PluginProcessor()
{
    fifo.setSize (2, 44100);

    samplesPerPixel  = addExtParam ("samplesPerPixel", "Samp/px",       "", "", {1.0f,   48.0f,  1.0f, 1.0f}, 1.0f, 0.0f, intTextFunction);
    verticalZoom     = addExtParam ("zoom",            "Zoom",          "", "", {0.1f,   100.0f, 0.0f, 0.3f}, 1.0f, 0.0f);
    verticalOffsetL  = addExtParam ("offset_l",        "Offset L",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    verticalOffsetR  = addExtParam ("offset_r",        "Offset R",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    triggerChannel   = addExtParam ("trigger_chan",    "Trigger Chan",  "", "", {-1.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, chanTextFunction);
    triggerMode      = addExtParam ("trigger_mode",    "Trigger Mode",  "", "", {0.0f,   2.0f,   1.0f, 1.0f}, 1.0f, 0.0f, modeTextFunction);
    triggerLevel     = addExtParam ("trigger_level",   "Trigger Level", "", "", {-1.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tlTextFunction);
    triggerPos       = addExtParam ("trigger_pos",     "Trigger Pos",   "", "", { 0.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tpTextFunction);
    triggerRun       = addExtParam ("trigger_run",     "Trigger Run",   "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, runTextFunction);
    triggerReset     = addExtParam ("trigger_reset",   "Trigger Reset", "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, resetTextFunction);
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
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
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
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
