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

    noteLabel.setLookAndFeel (&labelLookAndFeel);
    noteLabel.setFont (juce::FontOptions (50));
    noteLabel.setJustificationType (juce::Justification::centred);

    centsLabel.setLookAndFeel (&labelLookAndFeel);
    centsLabel.setFont (juce::FontOptions (20));
    centsLabel.setJustificationType (juce::Justification::centred);

    pitchLabel.setLookAndFeel (&labelLookAndFeel);
    pitchLabel.setFont (juce::FontOptions (20));
    pitchLabel.setJustificationType (juce::Justification::centred);
    
    startTimerHz (4);
    
    setGridSize (5, 3);
}

PitchTrackAudioProcessorEditor::~PitchTrackAudioProcessorEditor()
{
    noteLabel.setLookAndFeel (nullptr);
    centsLabel.setLookAndFeel (nullptr);
    pitchLabel.setLookAndFeel (nullptr);
}

//==============================================================================
void PitchTrackAudioProcessorEditor::paint (juce::Graphics& g)
{
    gin::ProcessorEditor::paint (g);
}

void PitchTrackAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto r = getFullGridArea().reduced (20);

    centsDisplay.setBounds (r.removeFromBottom (30));
    centsLabel.setBounds (r.removeFromBottom (30));
    pitchLabel.setBounds (r.removeFromBottom (30));

    noteLabel.setBounds (r);
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
