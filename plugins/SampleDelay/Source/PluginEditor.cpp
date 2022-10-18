#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SampleDelayAudioProcessorEditor::SampleDelayAudioProcessorEditor (SampleDelayAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    for (auto pp : p.getPluginParameters())
    {
        gin::ParamComponent* pc;
        
        if (pp == proc.mode)
            pc = new gin::Select (pp);
        else if (pp->isOnOff())
            pc = new gin::Switch (pp);
        else
            pc = new gin::Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (6, 1);
    
    proc.mode->addListener (this);
	proc.link->addListener (this);

    valueUpdated (proc.mode);
}

SampleDelayAudioProcessorEditor::~SampleDelayAudioProcessorEditor()
{
    proc.mode->removeListener (this);
	proc.link->removeListener (this);
}

//==============================================================================
void SampleDelayAudioProcessorEditor::resized()
{
	gin::ProcessorEditor::resized();

	componentForParam (*proc.mode)->setBounds (getGridArea (1, 0));
	componentForParam (*proc.link)->setBounds (getGridArea (2, 0));
	componentForParam (*proc.timeL)->setBounds (getGridArea (3, 0));
	componentForParam (*proc.samplesL)->setBounds (getGridArea (3, 0));
	componentForParam (*proc.timeR)->setBounds (getGridArea (4, 0));
	componentForParam (*proc.samplesR)->setBounds (getGridArea (4, 0));
	componentForParam (*proc.time)->setBounds (getGridArea (4, 0));
	componentForParam (*proc.samples)->setBounds (getGridArea (4, 0));
}

void SampleDelayAudioProcessorEditor::valueUpdated (gin::Parameter* param)
{
    if (param == proc.mode || param == proc.link)
    {
        bool samples = proc.mode->getUserValueInt() == 0;
		bool link = proc.link->getBoolValue();

		componentForParam (*proc.samples)->setVisible (link && samples);
		componentForParam (*proc.samplesL)->setVisible (! link && samples);
		componentForParam (*proc.samplesR)->setVisible (! link && samples);
		componentForParam (*proc.time)->setVisible (link && ! samples);
		componentForParam (*proc.timeL)->setVisible (! link && ! samples);
		componentForParam (*proc.timeR)->setVisible (! link && ! samples);
    }
}
