#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
static String onOffTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

static String modeTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Spectroscope";
        case 1:  return "Sonogram";
        default: return "";
    }
}

//==============================================================================
PluginProcessor::PluginProcessor()
{
    addExtParam (PARAM_MODE, "Mode", "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, modeTextFunction);
    addExtParam (PARAM_LOG,  "Log",  "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, onOffTextFunction);
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int)
{
    fifo.setSize (2, int (sampleRate));
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    if (fifo.getFreeSpace() >= buffer.getNumSamples())
        fifo.write (buffer);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
