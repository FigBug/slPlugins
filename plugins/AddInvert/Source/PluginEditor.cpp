#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AddInvertAudioProcessorEditor::AddInvertAudioProcessorEditor (AddInvertAudioProcessor& p)
    : gin::ProcessorEditor (p), aiProcessor (p)
{
    titleBar.setShowPresets (false);
    addAndMakeVisible (&scope);
    
    setGridSize (4, 2);
    
    scope.setNumChannels (2);
    scope.setNumSamplesPerPixel (2);
    scope.setVerticalZoomFactor (1.0f);

    scope.setColour (gin::TriggeredScope::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    scope.setColour (gin::TriggeredScope::traceColourId + 0, findColour (gin::PluginLookAndFeel::accentColourId));
    scope.setColour (gin::TriggeredScope::envelopeColourId + 0, juce::Colours::transparentBlack);
    scope.setColour (gin::TriggeredScope::traceColourId + 1, findColour (gin::PluginLookAndFeel::accentColourId));
    scope.setColour (gin::TriggeredScope::envelopeColourId + 1, juce::Colours::transparentBlack);
}

AddInvertAudioProcessorEditor::~AddInvertAudioProcessorEditor()
{
}

//==============================================================================
void AddInvertAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    scope.setBounds (getGridArea (0, 0, 4, 2));
}
