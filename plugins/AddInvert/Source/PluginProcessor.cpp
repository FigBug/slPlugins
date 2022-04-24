#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String abTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "A" : "B";
}

//==============================================================================
AddInvertAudioProcessor::AddInvertAudioProcessor()
{
    //==============================================================================
}

AddInvertAudioProcessor::~AddInvertAudioProcessor()
{
}

//==============================================================================
void AddInvertAudioProcessor::prepareToPlay (double, int)
{
}

void AddInvertAudioProcessor::releaseResources()
{
}

void AddInvertAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    const int numSamples = buffer.getNumSamples();
    
    float* aL = buffer.getWritePointer (0);
    float* aR = buffer.getWritePointer (1);
    float* bL = buffer.getWritePointer (2);
    float* bR = buffer.getWritePointer (3);

    juce::FloatVectorOperations::addWithMultiply (aL, bL, -1.0f, numSamples);
    juce::FloatVectorOperations::addWithMultiply (aR, bR, -1.0f, numSamples);
    
    if (editor != nullptr)
    {
        juce::AudioSampleBuffer outputBuffer (buffer.getArrayOfWritePointers(), 2, numSamples);
        editor->scope.addSamples (outputBuffer);
    }
}

//==============================================================================
bool AddInvertAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AddInvertAudioProcessor::createEditor()
{
    editor = new AddInvertAudioProcessorEditor (*this);
    return editor.getComponent();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AddInvertAudioProcessor();
}
