#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class MidiMonitorAudioProcessorEditor : public gin::ProcessorEditor,
                                        private juce::ChangeListener,
                                        private juce::Timer,
                                        private gin::Parameter::ParameterListener
{
public:
    MidiMonitorAudioProcessorEditor (MidiMonitorAudioProcessor&);
    ~MidiMonitorAudioProcessorEditor() override;

    void resized() override;

private:
    void changeListenerCallback (juce::ChangeBroadcaster*) override;
    void timerCallback() override;
    void valueUpdated (gin::Parameter*) override;

    void copyLogToClipboard();
    void saveLogToFile();

    void rebuildFilteredIndices();
    void syncFilterButtonsFromParams();
    bool messagePassesFilter (const juce::MidiMessage&) const;
    juce::String buildFilteredLogText() const;

    class MidiLogListBoxModel;

    MidiMonitorAudioProcessor& proc;

    std::unique_ptr<MidiLogListBoxModel> midiLogListBoxModel;
    juce::Array<int>            filteredIndices;
    juce::ListBox               midiLogListBox;
    juce::Label                 activeNotesLabel;
    juce::MidiKeyboardComponent keyboard { proc.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };
    gin::SVGButton              resetButton;
    gin::SVGButton              clearButton;
    gin::SVGButton              copyButton;
    gin::SVGButton              saveButton;
    juce::ToggleButton          filterNoteOnButton    { "Note On" };
    juce::ToggleButton          filterNoteOffButton   { "Note Off" };
    juce::ToggleButton          filterCCButton        { "CC" };
    juce::ToggleButton          filterPitchBendButton { "Pitch Bend" };
    juce::ToggleButton          filterPressureButton  { "Pressure" };
    juce::ToggleButton          filterSystemButton    { "System" };
    juce::ToggleButton          filterOtherButton     { "Other" };
    std::shared_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMonitorAudioProcessorEditor)
};
