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
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp == proc.beat)
            pc = new Select (pp);
        else if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (5, 1);
    
    proc.sync->addListener (this);
    
    valueUpdated (proc.sync);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
    proc.sync->removeListener (this);
}

//==============================================================================
void DelayAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    componentForParam (*proc.sync)->setBounds (getGridArea (0, 0));
    componentForParam (*proc.time)->setBounds (getGridArea (1, 0));
    componentForParam (*proc.beat)->setBounds (getGridArea (1, 0));
    componentForParam (*proc.fb)->setBounds (getGridArea (2, 0));
    componentForParam (*proc.cf)->setBounds (getGridArea (3, 0));
    componentForParam (*proc.mix)->setBounds (getGridArea (4, 0));
}

void DelayAudioProcessorEditor::valueUpdated (Parameter* param)
{
    if (param == proc.sync)
    {
        bool on = proc.sync->isOn();
        componentForParam (*proc.beat)->setVisible (on);
        componentForParam (*proc.time)->setVisible (! on);
    }
}
