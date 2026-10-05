#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (display);
    display.onContextMenu = [this] { showScaleMenu(); };

    setGridSize (8, 4);
    makeResizable (6 * cx + 2 * inset, headerHeight + 3 * cy, 2000, 1500);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::addScaleItems (juce::PopupMenu& m)
{
    const int current = proc.getScale();

    m.addItem ("dBFS", true, current == PluginProcessor::dbfs, [this] { proc.setScale (PluginProcessor::dbfs); });
    m.addItem ("K-20", true, current == PluginProcessor::k20,  [this] { proc.setScale (PluginProcessor::k20); });
    m.addItem ("K-14", true, current == PluginProcessor::k14,  [this] { proc.setScale (PluginProcessor::k14); });
    m.addItem ("K-12", true, current == PluginProcessor::k12,  [this] { proc.setScale (PluginProcessor::k12); });
}

void PluginEditor::addMenuItems (juce::PopupMenu& m)
{
    juce::PopupMenu sub;
    addScaleItems (sub);
    m.addSubMenu ("Scale", sub);
    m.addItem ("Reset Meters", [this] { proc.resetMeters(); });
}

void PluginEditor::showScaleMenu()
{
    juce::PopupMenu m;
    m.addSectionHeader ("Scale");
    addScaleItems (m);
    m.addSeparator();
    m.addItem ("Reset Meters", [this] { proc.resetMeters(); });
    m.setLookAndFeel (&getLookAndFeel());
    m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this).withMousePosition());
}

//==============================================================================
void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    display.setBounds (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);
}
