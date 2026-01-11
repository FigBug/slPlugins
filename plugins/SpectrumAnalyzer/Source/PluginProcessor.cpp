#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String onOffTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

juce::String modeTextFunction (const gin::Parameter&, float v)
{
    switch (int (v))
    {
        case 0:  return "Spectroscope";
        case 1:  return "Sonogram";
        default: return "";
    }
}

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

PluginProcessor::PluginProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    addExtParam (PARAM_MODE, "Mode", "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, modeTextFunction);
    addExtParam (PARAM_LOG,  "Log",  "", "", {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, onOffTextFunction);

    init();
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

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    juce::ScopedLock sl (lock);
    if (editor != nullptr)
    {
        const int num = buffer.getNumSamples();
        editor->scopeL.copySamples (buffer.getReadPointer (0), num);
        editor->sonogramL.copySamples (buffer.getReadPointer (0), num);
        
        if (getTotalNumInputChannels() > 1)
        {
            editor->scopeR.copySamples (buffer.getReadPointer (1), num);
            editor->sonogramR.copySamples (buffer.getReadPointer (1), num);
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
    editor = new PluginEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
