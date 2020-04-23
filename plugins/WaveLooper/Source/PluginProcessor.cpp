#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace gin;

//==============================================================================
WaveLooperAudioProcessor::WaveLooperAudioProcessor()
{
}

WaveLooperAudioProcessor::~WaveLooperAudioProcessor()
{
}

//==============================================================================
void WaveLooperAudioProcessor::stateUpdated()
{
    
}

void WaveLooperAudioProcessor::updateState()
{
    
}

void WaveLooperAudioProcessor::setFile (const File& f)
{
    
}

void WaveLooperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    GinProcessor::prepareToPlay (sampleRate, samplesPerBlock);
    
    oscillator.setSampleRateAndBlockSize (sampleRate, samplesPerBlock);
}

void WaveLooperAudioProcessor::releaseResources()
{
}

void WaveLooperAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    if (sample != nullptr)
    {
        oscillator.read (sample->getRootNote(), buffer, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool WaveLooperAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* WaveLooperAudioProcessor::createEditor()
{
    return new WaveLooperAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
WaveLooperAudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveLooperAudioProcessor();
}
