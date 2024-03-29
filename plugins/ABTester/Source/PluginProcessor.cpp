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
static juce::String abTextFunction (const Parameter&, float v)
{
    return v > 0.0f ? "B" : "A";
}

//==============================================================================
ABTesterAudioProcessor::ABTesterAudioProcessor()
{
    //==============================================================================
    addExtParam (PARAM_AB,    "A / B", "", "",    { 0.0f,   1.0f, 1.0f, 1.0f}, 0.0f, 0.0f, abTextFunction);
    addExtParam (PARAM_LEVEL, "Level", "", "dB",  {-100.0f, 6.0f, 0.0f, 5.0f}, 0.0f, 0.0f);
}

ABTesterAudioProcessor::~ABTesterAudioProcessor()
{
}

//==============================================================================
void ABTesterAudioProcessor::prepareToPlay (double sampleRate, int)
{
    aVal.reset (sampleRate, 0.05);
    bVal.reset (sampleRate, 0.05);
}

void ABTesterAudioProcessor::releaseResources()
{
}

void ABTesterAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    aVal.setTargetValue (parameterIntValue (PARAM_AB) == 0 ? juce::Decibels::decibelsToGain (parameterValue (PARAM_LEVEL)) : 0);
    bVal.setTargetValue (parameterIntValue (PARAM_AB) == 1 ? juce::Decibels::decibelsToGain (parameterValue (PARAM_LEVEL)) : 0);

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    if (numChannels >= 2)
    {
        // Apply A gain
        float* aL = buffer.getWritePointer (0);
        float* aR = buffer.getWritePointer (1);
        for (int s = 0; s < numSamples; s++)
        {
            float g = aVal.getNextValue();
            aL[s] *= g;
            aR[s] *= g;
        }
    }
    if (numChannels >= 4)
    {
        // Apply B gain
        float* aL = buffer.getWritePointer (0);
        float* aR = buffer.getWritePointer (1);
        float* bL = buffer.getWritePointer (2);
        float* bR = buffer.getWritePointer (3);
        for (int s = 0; s < numSamples; s++)
        {
            float g = bVal.getNextValue();
            aL[s] += bL[s] * g;
            aR[s] += bR[s] * g;
        }
    }
}

//==============================================================================
bool ABTesterAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ABTesterAudioProcessor::createEditor()
{
    editor = new ABTesterAudioProcessorEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ABTesterAudioProcessor();
}
