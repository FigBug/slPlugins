
#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::String enableTextFunction (const gin::Parameter&, float v)
{
    return v > 0.0f ? "On" : "Off";
}

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

CrossfeedAudioProcessor::CrossfeedAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    enable = addExtParam ("enable",    "Enable", "", "",    { 0.0f,   1.0f, 1.0f, 1.0f}, 1.0f, 0.0f, enableTextFunction);

    init();
}

CrossfeedAudioProcessor::~CrossfeedAudioProcessor()
{
}

//==============================================================================
void CrossfeedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    crossfeed_init (&crossfeed, (int)sampleRate);
    
    scratch.setSize (2, samplesPerBlock);
    
    enableVal.reset (sampleRate, 0.05);
    disableVal.reset (sampleRate, 0.05);
}

void CrossfeedAudioProcessor::releaseResources()
{
}

void CrossfeedAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    enableVal.setTargetValue (enable->getUserValue() > 0.5f ? 1.0f : 0.0f);
    disableVal.setTargetValue (enable->getUserValue() > 0.5f ? 0.0f : 1.0f);
    
    scratch.makeCopyOf (buffer, true);
    
    crossfeed_filter_inplace_noninterleaved (&crossfeed, scratch.getWritePointer (0),
                                             scratch.getWritePointer (1), (unsigned int)scratch.getNumSamples());
    
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

juce::AudioProcessorEditor* CrossfeedAudioProcessor::createEditor()
{
    return new CrossfeedAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrossfeedAudioProcessor();
}

