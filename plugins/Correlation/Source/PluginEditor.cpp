#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (display);
    display.onContextMenu = [this] { showSpeedMenu(); };

    setGridSize (8, 2);
    makeResizable (4 * cx + 2 * inset, headerHeight + cy, 2000, 600);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::addSpeedItems (juce::PopupMenu& m)
{
    const int current = proc.getSpeed();

    m.addItem ("Fast (100 ms)",   true, current == PluginProcessor::fast,   [this] { proc.setSpeed (PluginProcessor::fast); });
    m.addItem ("Medium (300 ms)", true, current == PluginProcessor::medium, [this] { proc.setSpeed (PluginProcessor::medium); });
    m.addItem ("Slow (1 s)",      true, current == PluginProcessor::slow,   [this] { proc.setSpeed (PluginProcessor::slow); });
}

void PluginEditor::addMenuItems (juce::PopupMenu& m)
{
    juce::PopupMenu sub;
    addSpeedItems (sub);
    m.addSubMenu ("Response", sub);
}

void PluginEditor::showSpeedMenu()
{
    juce::PopupMenu m;
    m.addSectionHeader ("Response");
    addSpeedItems (m);
    m.setLookAndFeel (&getLookAndFeel());
    m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this).withMousePosition());
}

//==============================================================================
void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    display.setBounds (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);
}
