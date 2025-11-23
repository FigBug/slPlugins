#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrossfeedAudioProcessorEditor::CrossfeedAudioProcessorEditor (CrossfeedAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p), meter (p.getOutputLevel())
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
    
    addAndMakeVisible (&meter);
    
    setGridSize (7, 1);
}

CrossfeedAudioProcessorEditor::~CrossfeedAudioProcessorEditor()
{
}

//==============================================================================
void CrossfeedAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll (Colours::white);
}

void CrossfeedAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();
    
    componentForParam (*proc.enable)->setBounds (getGridArea (3, 0));
}
