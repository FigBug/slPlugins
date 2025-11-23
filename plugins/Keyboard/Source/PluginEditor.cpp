#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KeyboardAudioProcessorEditor::KeyboardAudioProcessorEditor (KeyboardAudioProcessor& p)
  : gin::ProcessorEditor (p), proc (p),
    keyboard (processor.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    modWheel (processor.controllerState, 1), pitchWheel (processor.controllerState, -1)
{
    setGridSize (10, 2);
    
    addAndMakeVisible (keyboard);
    addAndMakeVisible (modWheel);
    addAndMakeVisible (pitchWheel);
}

KeyboardAudioProcessorEditor::~KeyboardAudioProcessorEditor()
{
}

//==============================================================================
void KeyboardAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    keyboard.setBounds (getGridArea (1, 0, 9, 2));
    
    auto rc = getGridArea (0, 0, 1, 2).withTrimmedRight (4);
    int w = (rc.getWidth() - 4) / 2;
    
    pitchWheel.setBounds (rc.removeFromLeft (w));
    modWheel.setBounds (rc.removeFromRight (w));
}
