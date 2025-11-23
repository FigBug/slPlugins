#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveLooperAudioProcessorEditor::WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        auto pc = new gin::Knob (pp);
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
    gin::ProcessorEditor::resized();
    
    thumbnail.setBounds (getGridArea (0, 0, 8, 1));
}
