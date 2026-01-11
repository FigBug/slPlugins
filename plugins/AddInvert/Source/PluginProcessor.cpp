#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
juce::String abTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "A" : "B";
}

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

AddInvertAudioProcessor::AddInvertAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    init();
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

void AddInvertAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    const int numChans = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    float* aL = (numChans >= 1) ? buffer.getWritePointer (0) : nullptr;
    float* aR = (numChans >= 2) ? buffer.getWritePointer (1) : nullptr;
    float* bL = (numChans >= 3) ? buffer.getWritePointer (2) : nullptr;
    float* bR = (numChans >= 4) ? buffer.getWritePointer (3) : nullptr;

    if (aL && bL) juce::FloatVectorOperations::addWithMultiply (aL, bL, -1.0f, numSamples);
    if (bL && bR) juce::FloatVectorOperations::addWithMultiply (aR, bR, -1.0f, numSamples);
    
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
