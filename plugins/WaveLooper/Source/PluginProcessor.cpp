#include "PluginProcessor.h"
#include "PluginEditor.h"

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

void WaveLooperAudioProcessor::setFile (const juce::File& f)
{
    
}

void WaveLooperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
    
    oscillator.setSampleRateAndBlockSize (sampleRate, samplesPerBlock);
}

void WaveLooperAudioProcessor::releaseResources()
{
}

void WaveLooperAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
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

juce::AudioProcessorEditor* WaveLooperAudioProcessor::createEditor()
{
    return new WaveLooperAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveLooperAudioProcessor();
}
