#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioProcessorEditor::AudioProcessorEditor (AudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        auto pc = new gin::Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (7, 1);
}

AudioProcessorEditor::~AudioProcessorEditor()
{
}

//==============================================================================
void AudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    componentForParam (*proc.room)->setBounds (getGridArea (0, 0));
    componentForParam (*proc.damp)->setBounds (getGridArea (1, 0));
    componentForParam (*proc.preDelay)->setBounds (getGridArea (2, 0));
    componentForParam (*proc.lp)->setBounds (getGridArea (3, 0));
    componentForParam (*proc.hp)->setBounds (getGridArea (4, 0));
    componentForParam (*proc.wet)->setBounds (getGridArea (5, 0));
    componentForParam (*proc.dry)->setBounds (getGridArea (6, 0));
}
