#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace gin;

//==============================================================================
WaveLooperAudioProcessorEditor::WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor& p)
    : GinAudioProcessorEditor (p), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        auto pc = new Knob (pp);
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    addAndMakeVisible (thumbnail);
    
    setGridSize (8, 1);
}

WaveLooperAudioProcessorEditor::~WaveLooperAudioProcessorEditor()
{
}

//==============================================================================
void WaveLooperAudioProcessorEditor::refresh()
{
}

void WaveLooperAudioProcessorEditor::resized()
{
    GinAudioProcessorEditor::resized();
    
    thumbnail.setBounds (getGridArea (0, 0, 8, 1));
}
