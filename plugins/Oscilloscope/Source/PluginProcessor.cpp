
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String onOffTextFunction (const gin::Parameter& p)
{
    return p.getUserValue() > 0.0f ? "On" : "Off";
}

juce::String chanTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case -1: return "Ave";
        case 0:  return "Left";
        case 1:  return "Right";
        default: return "";
    }
}

juce::String modeTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Off";
        case 1:  return "Up";
        case 2:  return "Down";
        default: return "";
    }
}

juce::String intTextFunction (const gin::Parameter&, float v)
{
    return juce::String (int (v));
}

juce::String tlTextFunction (const gin::Parameter&, float v)
{
    return juce::String (v, 2);
}

juce::String runTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Normal";
        case 1:  return "Single";
        default: return "";
    }
}

juce::String resetTextFunction (const gin::Parameter&, float)
{
    return "Reset";
}

//==============================================================================
PluginProcessor::PluginProcessor()
{
    fifo.setSize (2, 44100);

    addExtParam (PARAM_SAMPLES_PER_PIXEL, "Samp/px",       "", "", {1.0f,   48.0f,  1.0f, 1.0f}, 1.0f, 0.0f, intTextFunction);
    addExtParam (PARAM_VERTICAL_ZOOM,     "Zoom",          "", "", {0.1f,   100.0f, 0.0f, 0.3f}, 1.0f, 0.0f);
    addExtParam (PARAM_VERTICAL_OFFSET_L, "Offset L",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    addExtParam (PARAM_VERTICAL_OFFSET_R, "Offset R",      "", "", {-2.0f,  2.0f,   0.0f, 1.0f}, 0.0f, 0.0f);
    addExtParam (PARAM_TRIGGER_CHANNEL,   "Trigger Chan",  "", "", {-1.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, chanTextFunction);
    addExtParam (PARAM_TRIGGER_MODE,      "Trigger Mode",  "", "", {0.0f,   2.0f,   1.0f, 1.0f}, 1.0f, 0.0f, modeTextFunction);
    addExtParam (PARAM_TRIGGER_LEVEL,     "Trigger Level", "", "", {-1.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tlTextFunction);
    addExtParam (PARAM_TRIGGER_POS,       "Trigger Pos",   "", "", { 0.0f,  1.0f,   0.0f, 1.0f}, 0.0f, 0.0f, tlTextFunction);
    addExtParam (PARAM_TRIGGER_RUN,       "Trigger Run",   "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, runTextFunction);
    addExtParam (PARAM_TRIGGER_RESET,     "Trigger Reset", "", "", { 0.0f,  1.0f,   1.0f, 1.0f}, 0.0f, 0.0f, resetTextFunction);
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::numChannelsChanged()
{
    fifo.setSize (getTotalNumInputChannels(), 44100);
}

void PluginProcessor::prepareToPlay (double, int)
{
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
