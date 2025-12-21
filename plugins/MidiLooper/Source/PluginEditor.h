#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class MidiLooperAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    MidiLooperAudioProcessorEditor (MidiLooperAudioProcessor&);
    ~MidiLooperAudioProcessorEditor() override;

    void resized() override;

private:
    void updatePlayStopButtons();
    void showMenu();

    MidiLooperAudioProcessor& proc;

    gin::MidiFileComponent  midiFile { proc.midiPlayer };
    gin::SVGButton          midiPlay;
    gin::SVGButton          midiStop;
    gin::SVGButton          midiMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLooperAudioProcessorEditor)
};
