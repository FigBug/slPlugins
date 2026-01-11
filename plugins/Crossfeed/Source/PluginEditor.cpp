#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrossfeedAudioProcessorEditor::CrossfeedAudioProcessorEditor (CrossfeedAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
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

    spectrum.setColour (gin::SpectrumAnalyzer::lineColourId, findColour (gin::GinLookAndFeel::grey45ColourId));
    spectrum.setColour (gin::SpectrumAnalyzer::gridColourId, findColour (gin::GinLookAndFeel::grey45ColourId).withMultipliedAlpha (0.2f));
    spectrum.setDrawGrid (true);

    auto accent = findColour (gin::GinLookAndFeel::accentColourId);
    for (int i = 0; i < 2; ++i)
    {
        auto c = accent.withRotatedHue (-0.05f + i * 0.1f);
        spectrum.setColour (gin::SpectrumAnalyzer::traceColourId + i, c);
        spectrum.setColour (gin::SpectrumAnalyzer::envelopeColourId + i, c.withAlpha (0.5f));
    }

    spectrum.setNumChannels (2);
    addAndMakeVisible (spectrum);

    setGridSize (7, 1);
}

CrossfeedAudioProcessorEditor::~CrossfeedAudioProcessorEditor()
{
}

//==============================================================================
void CrossfeedAudioProcessorEditor::paint (juce::Graphics& g)
{
    gin::ProcessorEditor::paint (g);
}

void CrossfeedAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();
    
    componentForParam (*proc.enable)->setBounds (getGridArea (0, 0));
    spectrum.setBounds (getGridArea (1, 0, 6, 1));
}
