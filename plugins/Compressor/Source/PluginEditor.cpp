/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
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

    addAndMakeVisible (meter);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (reductionMeter);
    reductionMeter.setTopDown (true);

    addAndMakeVisible (scope);
    scope.setNumChannels (3);
    scope.setTriggerMode (gin::TriggeredScope::None);
    scope.setNumSamplesPerPixel (256);
    scope.setVerticalZoomFactor (2.0);
    scope.setVerticalZoomOffset (-0.5, 0);
    scope.setVerticalZoomOffset (-0.5, 1);
    scope.setVerticalZoomOffset (-0.5, 2);
    scope.setColour (gin::TriggeredScope::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    scope.setColour (gin::TriggeredScope::traceColourId + 0, juce::Colours::transparentBlack);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 0, findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.3f));
    scope.setColour (gin::TriggeredScope::traceColourId + 1, findColour (gin::PluginLookAndFeel::whiteColourId).withAlpha (0.7f));
    scope.setColour (gin::TriggeredScope::envelopeColourId + 1, findColour (gin::PluginLookAndFeel::whiteColourId).withAlpha (0.7f));
    scope.setColour (gin::TriggeredScope::traceColourId + 2, findColour (gin::PluginLookAndFeel::accentColourId));
    scope.setColour (gin::TriggeredScope::envelopeColourId + 2, juce::Colours::transparentBlack);

    inputMeter.setColour (gin::LevelMeter::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    inputMeter.setColour (gin::LevelMeter::meterColourId, findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.3f));
    outputMeter.setColour (gin::LevelMeter::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    outputMeter.setColour (gin::LevelMeter::meterColourId, findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.3f));
    reductionMeter.setColour (gin::LevelMeter::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    reductionMeter.setColour (gin::LevelMeter::meterColourId, findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.3f));

    meter.setColour (gin::DynamicsMeter::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    meter.setColour (gin::DynamicsMeter::meterColourId, findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.3f));

    setGridSize (7, 2);

    for (auto pp : proc.getPluginParameters())
        pp->addListener (this);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    for (auto p : proc.getPluginParameters())
        p->removeListener (this);
}

//==============================================================================
void CompressorAudioProcessorEditor::valueUpdated (gin::Parameter*)
{
    meter.repaint();
}

void CompressorAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    componentForParam (*proc.input)->setBounds (getGridArea (0, 0));
    componentForParam (*proc.attack)->setBounds (getGridArea (1, 0));
    componentForParam (*proc.release)->setBounds (getGridArea (2, 0));
    componentForParam (*proc.ratio)->setBounds (getGridArea (3, 0));
    componentForParam (*proc.threshold)->setBounds (getGridArea (4, 0));
    componentForParam (*proc.knee)->setBounds (getGridArea (5, 0));
    componentForParam (*proc.output)->setBounds (getGridArea (6, 0));

    auto rc = getGridArea (0, 1, 7, 1);

    inputMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);

    meter.setBounds (rc.removeFromLeft (rc.getHeight()));
    rc.removeFromLeft (4);

    reductionMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);

    outputMeter.setBounds (rc.removeFromLeft (16));
    rc.removeFromLeft (4);

    scope.setBounds (rc);
}
