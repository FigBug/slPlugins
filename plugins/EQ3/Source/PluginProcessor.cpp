#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
EQ3AudioProcessor::EQ3AudioProcessor()
{
    fifo.setSize (3, 44100);

    attack    = addExtParam ("attack",    "Attack",    "", "ms",    { 0.0f,     5.0f, 0.0f, 1.0f},    0.0f, 0.1f);
    release   = addExtParam ("release",   "Release",   "", "ms",    { 1.0f,   500.0f, 0.0f, 0.3f},    5.0f, 0.1f);
    threshold = addExtParam ("threshold", "Threshold", "", "dB",    { -60.0f,   0.0f, 0.0f, 1.0f},  -30.0f, 0.1f);
    input     = addExtParam ("input",     "Input",     "", "dB",    { -30.0f,  30.0f, 0.0f, 1.0f},    0.0f, 0.1f);
    output    = addExtParam ("output",    "Output",    "", "dB",    { -30.0f,  30.0f, 0.0f, 1.0f},    0.0f, 0.1f);

    attack->conversionFunction  = [] (float in) { return in / 1000.0; };
    release->conversionFunction = [] (float in) { return in / 1000.0; };
    input->conversionFunction   = [] (float in) { return juce::Decibels::decibelsToGain (in); };
    output->conversionFunction  = [] (float in) { return juce::Decibels::decibelsToGain (in); };
}

EQ3AudioProcessor::~EQ3AudioProcessor()
{
}

//==============================================================================
void EQ3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    gate.setSampleRate (sampleRate);
    gate.reset();
    gate.setMode (gin::Dynamics::gate);
    gate.setNumChannels (getTotalNumInputChannels());
}

void EQ3AudioProcessor::reset()
{
    gin::Processor::reset();

    gate.reset();
}

void EQ3AudioProcessor::numChannelsChanged()
{
    gate.setNumChannels (getTotalNumInputChannels());
}

void EQ3AudioProcessor::releaseResources()
{
}

void EQ3AudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();

    gin::ScratchBuffer fifoData (3, numSamples);
    if (getTotalNumInputChannels() == 2)
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (0), buffer.getReadPointer (0), numSamples);
        juce::FloatVectorOperations::add (fifoData.getWritePointer (0), buffer.getReadPointer (1), numSamples);
        juce::FloatVectorOperations::multiply (fifoData.getWritePointer (0), 0.5, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (0), buffer.getReadPointer (0), numSamples);
    }

    gin::ScratchBuffer envData (1, numSamples);

    if (isSmoothing())
    {
        int pos = 0;
        while (pos < numSamples)
        {
            auto workBuffer = gin::sliceBuffer (buffer, pos, 1);
            auto envWorkBuffer = gin::sliceBuffer (envData, pos, 1);

            gate.setInputGain (input->getProcValue (1));
            gate.setOutputGain (output->getProcValue (1));
            gate.setParams (attack->getProcValue (1), 0,
                               release->getProcValue (1),
                               threshold->getProcValue (1),
                               1000,
                               0);

            gate.process (workBuffer, &envWorkBuffer);

            pos++;
        }

    }
    else
    {
        gate.setInputGain (input->getProcValue (numSamples));
        gate.setOutputGain (output->getProcValue (numSamples));
        gate.setParams (attack->getProcValue (numSamples), 0,
                           release->getProcValue (numSamples),
                           threshold->getProcValue (numSamples),
                           1000,
                           0);

        gate.process (buffer, &envData);
    }

    if (getTotalNumInputChannels() == 2)
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (1), buffer.getReadPointer (0), numSamples);
        juce::FloatVectorOperations::add (fifoData.getWritePointer (1), buffer.getReadPointer (1), numSamples);
        juce::FloatVectorOperations::multiply (fifoData.getWritePointer (1), 0.5, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::copy (fifoData.getWritePointer (1), buffer.getReadPointer (0), numSamples);
    }

    juce::FloatVectorOperations::copy (fifoData.getWritePointer (2), envData.getReadPointer (0), numSamples);

    if (fifo.getFreeSpace() >= numSamples)
        fifo.write (fifoData);
}

//==============================================================================
bool EQ3AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* EQ3AudioProcessor::createEditor()
{
    return new EQ3AudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EQ3AudioProcessor();
}
