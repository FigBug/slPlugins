/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

using namespace gin;

//==============================================================================
String onOffTextFunction (const Parameter& p)
{
    return p.getUserValue() > 0.0f ? "On" : "Off";
}

String chanTextFunction (const Parameter&, float v)
{
    switch (int (v))
    {
        case -1: return "Ave";
        case 0:  return "Left";
        case 1:  return "Right";
        default: return "";
    }
}

String modeTextFunction (const Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Off";
        case 1:  return "Up";
        case 2:  return "Down";
        default: return "";
    }
}

String intTextFunction (const Parameter&, float v)
{
    return String (int (v));
}

String tlTextFunction (const Parameter&, float v)
{
    return String (v, 2);
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
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double, int)
{
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
