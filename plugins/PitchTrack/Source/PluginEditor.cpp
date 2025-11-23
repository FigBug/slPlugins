#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchTrackAudioProcessorEditor::PitchTrackAudioProcessorEditor (PitchTrackAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (&pitch);

    pitch.setFont (50);
    pitch.setJustificationType (juce::Justification::centred);
    
    startTimerHz (4);
    
    setGridSize (7, 3);
}

PitchTrackAudioProcessorEditor::~PitchTrackAudioProcessorEditor()
{
}

//==============================================================================
void PitchTrackAudioProcessorEditor::paint (juce::Graphics& g)
{
    gin::ProcessorEditor::paint (g);
}

void PitchTrackAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto r = getFullGridArea();
    
    pitch.setBounds (r);
}

void PitchTrackAudioProcessorEditor::timerCallback()
{
    if (proc.getPitch() != lastPitch)
    {
        lastPitch = proc.getPitch();
        pitch.setText (juce::String::formatted ("%.1f Hz", lastPitch), juce::dontSendNotification);
        
        DBG(lastPitch);
    }
}
