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
ABTesterAudioProcessorEditor::ABTesterAudioProcessorEditor (ABTesterAudioProcessor& p)
    : gin::ProcessorEditor (p), abProcessor (p)
{
    for (auto pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (6, 1);
}

ABTesterAudioProcessorEditor::~ABTesterAudioProcessorEditor()
{
}

//==============================================================================
void ABTesterAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    componentForId (PARAM_AB)->setBounds (getGridArea (2, 0));
    componentForId (PARAM_LEVEL)->setBounds (getGridArea (3, 0));
}
