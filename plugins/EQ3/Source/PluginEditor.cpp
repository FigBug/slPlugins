#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQ3AudioProcessorEditor::EQ3AudioProcessorEditor (EQ3AudioProcessor& p)
    : gin::ProcessorEditor (p, 60, 100), proc (p)
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
    scope.setColour (gin::TriggeredScope::traceColourId + 0, juce::Colours::transparentBlack);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 0, juce::Colours::orange);
    scope.setColour (gin::TriggeredScope::traceColourId + 1, juce::Colours::transparentBlack);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 1, juce::Colours::white);
    scope.setColour (gin::TriggeredScope::traceColourId + 2, juce::Colours::red);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 2, juce::Colours::transparentBlack);

    setGridSize (7, 2);

    for (auto pp : proc.getPluginParameters())
        pp->addListener (this);
}

EQ3AudioProcessorEditor::~EQ3AudioProcessorEditor()
{
    for (auto p : proc.getPluginParameters())
        p->removeListener (this);
}

//==============================================================================
void EQ3AudioProcessorEditor::valueUpdated (gin::Parameter*)
{
    meter.repaint();
}

void EQ3AudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    componentForParam (*proc.input)->setBounds (getGridArea (0, 0));
    componentForParam (*proc.attack)->setBounds (getGridArea (2, 0));
    componentForParam (*proc.release)->setBounds (getGridArea (3, 0));
    componentForParam (*proc.threshold)->setBounds (getGridArea (4, 0));
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
