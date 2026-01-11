#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchTrackAudioProcessorEditor::PitchTrackAudioProcessorEditor (PitchTrackAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (noteLabel);
    addAndMakeVisible (centsLabel);
    addAndMakeVisible (pitchLabel);
    addAndMakeVisible (centsDisplay);

    noteLabel.setFont (juce::FontOptions (200));
    noteLabel.setJustificationType (juce::Justification::centred);

    centsLabel.setFont (juce::FontOptions (50));
    centsLabel.setJustificationType (juce::Justification::centred);

    pitchLabel.setFont (juce::FontOptions (50));
    pitchLabel.setJustificationType (juce::Justification::centred);
    
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

    auto r = getFullGridArea().withSizeKeepingCentre (75, 75);

    noteLabel.setBounds (r);

    r = r.removeFromBottom (25).translated (0, 25);
    centsLabel.setBounds (r);

    r = r.translated (0, 25);
    pitchLabel.setBounds (r);

    r = r.translated (0, 25).withSizeKeepingCentre (75, 25);
    centsDisplay.setBounds (r);
}

void PitchTrackAudioProcessorEditor::timerCallback()
{
    auto pitch = proc.getPitch();

    if (pitch > 0)
    {
        auto note = gin::getMidiNoteFromHertz (pitch);
        auto centsOffset = (note - juce::roundToInt (note)) * 100.0f;

        noteLabel.setText (juce::MidiMessage::getMidiNoteName (juce::roundToInt (note), true, true, 4), juce::dontSendNotification);
        centsLabel.setText (juce::String::formatted ("%d cents", juce::roundToInt (centsOffset)), juce::dontSendNotification);
        pitchLabel.setText (juce::String::formatted ("%.1f Hz", pitch), juce::dontSendNotification);
        centsDisplay.setCents (centsOffset);
    }
    else
    {
        noteLabel.setText ("-", juce::dontSendNotification);
        centsLabel.setText ("- cents", juce::dontSendNotification);
        pitchLabel.setText ("--- Hz", juce::dontSendNotification);
        centsDisplay.setCents (0.0f);
    }
}
