/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace gin;

//==============================================================================
FormulaAudioProcessorEditor::FormulaAudioProcessorEditor (FormulaAudioProcessor& p)
    : GinAudioProcessorEditor (p), processor (p)
{
    addAndMakeVisible (osc1);
    addAndMakeVisible (osc2);
    addAndMakeVisible (osc3);
    addAndMakeVisible (osc1Label);
    addAndMakeVisible (osc2Label);
    addAndMakeVisible (osc3Label);
    
    osc1.addListener (this);
    osc2.addListener (this);
    osc3.addListener (this);
    
    osc1.setText (processor.getOscFormula (0), dontSendNotification);
    osc2.setText (processor.getOscFormula (1), dontSendNotification);
    osc3.setText (processor.getOscFormula (2), dontSendNotification);
    
    addAndMakeVisible (&scope);
    
    for (Parameter* pp : p.getPluginParameters())
    {
        auto* pc = new Knob (pp);
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (6, 4);
    
    scope.setNumChannels (2);
    scope.setNumSamplesPerPixel (2);
    scope.setVerticalZoomFactor (1.0f);
}

FormulaAudioProcessorEditor::~FormulaAudioProcessorEditor()
{
}

//==============================================================================
void FormulaAudioProcessorEditor::refresh()
{
    osc1.setText (processor.getOscFormula (0), dontSendNotification);
    osc2.setText (processor.getOscFormula (1), dontSendNotification);
    osc3.setText (processor.getOscFormula (2), dontSendNotification);
}

void FormulaAudioProcessorEditor::resized()
{
    GinAudioProcessorEditor::resized();

    // Set position for formula editors
    auto rc = getGridArea (0, 0, 6, 1);
    const int h = rc.getHeight() / 3;
    rc = rc.withSizeKeepingCentre (rc.getWidth() - 4, rc.getHeight());
    
    auto rcLabels = rc.removeFromLeft (50);
    osc1Label.setBounds (rcLabels.removeFromTop (h));
    osc2Label.setBounds (rcLabels.removeFromTop (h));
    osc3Label.setBounds (rcLabels.removeFromTop (h));

    osc1.setBounds (rc.removeFromTop (h).reduced (0, 6));
    osc2.setBounds (rc.removeFromTop (h).reduced (0, 6));
    osc3.setBounds (rc.removeFromTop (h).reduced (0, 6));
    
    // Position the controls
    int x = 0; int y = 1;
    for (auto* c : controls)
    {
        c->setBounds (getGridArea (x, y));
        
        x++;
        if (x == 6)
        {
            x = 0;
            y++;
        }
    }
    
    // Position the scope
    scope.setBounds (getGridArea (0, 2, 6, 2));
}

void FormulaAudioProcessorEditor::textEditorReturnKeyPressed (TextEditor& ed)
{
    if (&ed == &osc1) processor.setOscFormula (0, osc1.getText());
    if (&ed == &osc2) processor.setOscFormula (1, osc1.getText());
    if (&ed == &osc3) processor.setOscFormula (2, osc1.getText());
}

void FormulaAudioProcessorEditor::textEditorEscapeKeyPressed (TextEditor&ed)
{
    if (&ed == &osc1) osc1.setText (processor.getOscFormula (0), dontSendNotification);
    if (&ed == &osc2) osc2.setText (processor.getOscFormula (1), dontSendNotification);
    if (&ed == &osc3) osc3.setText (processor.getOscFormula (2), dontSendNotification);
}

void FormulaAudioProcessorEditor::textEditorFocusLost (TextEditor& ed)
{
    textEditorReturnKeyPressed (ed);
}
