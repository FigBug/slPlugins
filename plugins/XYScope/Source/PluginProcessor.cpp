#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
PluginProcessor::PluginProcessor()
{
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

void PluginProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    if (fifo.getFreeSpace() >= buffer.getNumSamples())
        fifo.write (buffer);
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
