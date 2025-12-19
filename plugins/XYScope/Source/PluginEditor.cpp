#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (&scope);

    setGridSize (7, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();
    
    auto rc = juce::Rectangle<int> (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);
    
    scope.setBounds (rc);
}

