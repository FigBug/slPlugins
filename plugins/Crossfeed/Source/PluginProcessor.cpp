
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrossfeedAudioProcessor::CrossfeedAudioProcessor()
{
    enable = addPluginParameter (new slParameter (PARAM_ENABLE,       "Enable",       "",     0.0f,      1.0f, 1.0f,    1.0f, 1.0f));
}

CrossfeedAudioProcessor::~CrossfeedAudioProcessor()
{
}

//==============================================================================
void CrossfeedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    crossfeed_init (&crossfeed, sampleRate);
    
    scratch.setSize (2, samplesPerBlock);
    
    enableVal.reset (sampleRate, 0.05);
    disableVal.reset (sampleRate, 0.05);
}

void CrossfeedAudioProcessor::releaseResources()
{
}

void CrossfeedAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    enableVal.setValue (enable->getUserValue() > 0.5f ? 1.0f : 0.0f);
    disableVal.setValue (enable->getUserValue() > 0.5f ? 0.0f : 1.0f);
    
    scratch.makeCopyOf (buffer, true);
    
    crossfeed_filter_inplace_noninterleaved (&crossfeed, scratch.getWritePointer (0),
                                             scratch.getWritePointer (1), scratch.getNumSamples());
    
    gin::applyGain (buffer, disableVal);
    gin::applyGain (scratch, enableVal);
    
    buffer.addFrom (0, 0, scratch, 0, 0, buffer.getNumSamples());
    buffer.addFrom (1, 0, scratch, 1, 0, buffer.getNumSamples());
    
    outputLevel.trackBuffer (buffer);
}

//==============================================================================
bool CrossfeedAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* CrossfeedAudioProcessor::createEditor()
{
    return new CrossfeedAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrossfeedAudioProcessor();
}
