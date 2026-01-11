#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

CompressorAudioProcessor::CompressorAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    fifo.setSize (3, 44100);

    attack    = addExtParam ("attack",    "Attack",    "", "ms",   { 1.0f,   200.0f, 0.0f, 0.4f},    1.0f, 0.1f);
    release   = addExtParam ("release",   "Release",   "", "ms",   { 1.0f,  2000.0f, 0.0f, 0.4f},    5.0f, 0.1f);
    ratio     = addExtParam ("ratio",     "Ratio",     "", "",     { 1.0f,    30.0f, 0.0f, 0.4f},    5.0f, 0.1f);
    threshold = addExtParam ("threshold", "Threshold", "", "",     { -60.0f,   0.0f, 0.0f, 1.0f},  -30.0f, 0.1f);
    knee      = addExtParam ("knee",      "Knee",      "", "",     { 0.0f,    60.0f, 0.0f, 1.0f},    5.0f, 0.1f);
    input     = addExtParam ("input",     "Input",     "", "",     { -30.0f,  30.0f, 0.0f, 1.0f},    0.0f, 0.1f);
    output    = addExtParam ("output",    "Output",    "", "",     { -30.0f,  30.0f, 0.0f, 1.0f},    0.0f, 0.1f);
    
    attack->conversionFunction  = [] (float in) { return in / 1000.0; };
    release->conversionFunction = [] (float in) { return in / 1000.0; };
    input->conversionFunction   = [] (float in) { return juce::Decibels::decibelsToGain (in); };
    output->conversionFunction  = [] (float in) { return juce::Decibels::decibelsToGain (in); };

    for (int i = 0; i < BinaryData::namedResourceListSize; i++)
    {
        int sz = 0;
        if (auto data = BinaryData::getNamedResource (BinaryData::namedResourceList[i], sz))
            extractProgram (BinaryData::originalFilenames[i], juce::MemoryBlock (data, size_t (sz)));
    }

    init();
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);

    compressor.setSampleRate (sampleRate);
    compressor.reset();
    compressor.setNumChannels (getTotalNumInputChannels());
}

void CompressorAudioProcessor::reset()
{
    gin::Processor::reset();
    
    compressor.reset();
}

void CompressorAudioProcessor::numChannelsChanged()
{
    compressor.setNumChannels (getTotalNumInputChannels());
}

void CompressorAudioProcessor::releaseResources()
{
}

void CompressorAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

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
            
            compressor.setInputGain (input->getProcValue (1));
            compressor.setOutputGain (output->getProcValue (1));
            compressor.setParams (attack->getProcValue (1),
                                  0.0f,
                                  release->getProcValue (1),
                                  threshold->getProcValue (1),
                                  ratio->getProcValue (1),
                                  knee->getProcValue (1));
            
            compressor.process (workBuffer, &envWorkBuffer);
            
            pos++;
        }

    }
    else
    {
        compressor.setInputGain (input->getProcValue (numSamples));
        compressor.setOutputGain (output->getProcValue (numSamples));
        compressor.setParams (attack->getProcValue (numSamples),
                              0.0f,
                              release->getProcValue (numSamples),
                              threshold->getProcValue (numSamples),
                              ratio->getProcValue (numSamples),
                              knee->getProcValue (numSamples));

        compressor.process (buffer, &envData);
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
bool CompressorAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}

