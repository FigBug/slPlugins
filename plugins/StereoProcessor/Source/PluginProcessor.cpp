#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

static juce::String percentTextFunction (const gin::Parameter&, float v)
{
    return juce::String (juce::roundToInt (v * 100));
}

//==============================================================================
AudioProcessor::AudioProcessor()
    : gin::Processor (false, gin::ProcessorOptions().withAdditionalCredits ({"Michael \"LOSER\" Gruhn"}))
{
    width1      = addExtParam ("width1",    "Width",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    center1     = addExtParam ("center1",   "Center",    "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    pan1        = addExtParam ("pan1",      "Pan",       "", "%",   {  -1.0f,   1.0f, 0.0f, 1.0f}, 0.0f, 0.1f, percentTextFunction);
    rotation    = addExtParam ("rotation",  "Rotation",  "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    pan2        = addExtParam ("pan2",      "Pan",       "", "%",   {  -1.0f,   1.0f, 0.0f, 1.0f}, 0.0f, 0.1f, percentTextFunction);
    center2     = addExtParam ("center2",   "Center",    "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    width2      = addExtParam ("width2",    "Width",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    output      = addExtParam ("output",    "Output",    "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.5f, 0.1f, percentTextFunction);
    
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

void AudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();
    if (isSmoothing())
    {
        int pos = 0;

        while (pos < numSamples)
        {
            auto workBuffer = gin::sliceBuffer (buffer, pos, 1);

            proc.setParameters (width1->getProcValue (1), center1->getProcValue (1),
                                pan1->getProcValue (1), rotation->getProcValue (1),
                                pan2->getProcValue (1), center2->getProcValue (1),
                                width2->getProcValue (1), output->getProcValue (1));

            proc.process (workBuffer);
            pos++;
        }
    }
    else
    {
        proc.setParameters (width1->getProcValue (numSamples), center1->getProcValue (numSamples),
                            pan1->getProcValue (numSamples), rotation->getProcValue (numSamples),
                            pan2->getProcValue (numSamples), center2->getProcValue (numSamples),
                            width2->getProcValue (numSamples), output->getProcValue (numSamples));

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
