#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ABTesterAudioProcessorEditor::ABTesterAudioProcessorEditor (ABTesterAudioProcessor& p)
    : gin::ProcessorEditor (p), abProcessor (p)
{
    for (auto pp : p.getPluginParameters())
    {
        gin::ParamComponent* pc;
        
        if (pp->isOnOff())
            pc = new gin::Switch (pp);
        else
            pc = new gin::Knob (pp);
        
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
