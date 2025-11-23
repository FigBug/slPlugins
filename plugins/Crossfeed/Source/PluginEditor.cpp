#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrossfeedAudioProcessorEditor::CrossfeedAudioProcessorEditor (CrossfeedAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p), meter (p.getOutputLevel())
{
    for (auto pp : p.getPluginParameters())
    {
        auto k = new gin::Knob (pp);
        
        addAndMakeVisible (k);
        controls.add (k);
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
