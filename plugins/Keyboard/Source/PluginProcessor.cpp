#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
KeyboardAudioProcessor::KeyboardAudioProcessor()
{
}

KeyboardAudioProcessor::~KeyboardAudioProcessor()
{
}

//==============================================================================
void KeyboardAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void KeyboardAudioProcessor::releaseResources()
{
}

void KeyboardAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    controllerState.processBuffer (midi);
    keyboardState.processNextMidiBuffer (midi, 0, buffer.getNumSamples(), true);
}

//==============================================================================
bool KeyboardAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KeyboardAudioProcessor::createEditor()
{
    editor = new KeyboardAudioProcessorEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KeyboardAudioProcessor();
}
