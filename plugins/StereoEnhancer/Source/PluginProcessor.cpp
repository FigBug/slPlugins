#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

static juce::String percentTextFunction (const gin::Parameter&, float v)
{
    return juce::String (juce::roundToInt (v * 100));
}

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withAdditionalCredits ({"Michael \"LOSER\" Gruhn"})
        .withMidiLearn();
}

AudioProcessor::AudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    widthLP     = addExtParam ("widthLP",       "Width",    "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.5f, 0.1f, percentTextFunction);
    freqHPFader = addExtParam ("freqHPFader",   "Freq HP",  "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.5f, 0.1f, percentTextFunction);
    widthHP     = addExtParam ("widthHP",       "Width HP", "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f, 0.1f, percentTextFunction);

    for (int i = 0; i < BinaryData::namedResourceListSize; i++)
    {
        int sz = 0;
        if (auto data = BinaryData::getNamedResource (BinaryData::namedResourceList[i], sz))
            extractProgram (BinaryData::originalFilenames[i], juce::MemoryBlock (data, size_t (sz)));
    }

    init();
}

AudioProcessor::~AudioProcessor()
{
}

//==============================================================================
void AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
}

void AudioProcessor::reset()
{
    gin::Processor::reset();
}

void AudioProcessor::releaseResources()
{
}

void AudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
    if (isSmoothing())
    {
        int pos = 0;

        while (pos < numSamples)
        {
            auto workBuffer = gin::sliceBuffer (buffer, pos, 1);

            proc.setParameters (widthLP->getProcValue (1), freqHPFader->getProcValue (1), widthHP->getProcValue (1));

            proc.process (workBuffer);
            pos++;
        }
    }
    else
    {
        proc.setParameters (widthLP->getProcValue (numSamples), freqHPFader->getProcValue (numSamples), widthHP->getProcValue (numSamples));

        proc.process (buffer);
    }
}

//==============================================================================
bool AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioProcessor::createEditor()
{
    return new AudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessor();
}

