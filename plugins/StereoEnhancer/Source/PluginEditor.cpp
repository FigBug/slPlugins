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

    auto x = 2;
    for (auto pp : proc.getPluginParameters())
        componentForParam (*pp)->setBounds (getGridArea (x++, 0));
}
