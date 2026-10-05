#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (meterL);
    addAndMakeVisible (meterR);

    meterL.onContextMenu = [this] { showReferenceMenu(); };
    meterR.onContextMenu = [this] { showReferenceMenu(); };

    proc.state.addListener (this);
    updateCaptions();

    // Width from the grid, height from the meters: nothing taller than the meters themselves
    const int w = 10 * cx + 2 * inset;
    setSize (w, heightForWidth (w));
    makeResizable (w / 2, heightForWidth (w / 2), 2000, heightForWidth (2000));
}

PluginEditor::~PluginEditor()
{
    proc.state.removeListener (this);
}

//==============================================================================
static const float referenceLevels[] = { -24.0f, -20.0f, -18.0f, -14.0f, -12.0f, -10.0f, -8.0f };

void PluginEditor::addReferenceItems (juce::PopupMenu& m)
{
    const float current = proc.getReferenceDb();

    for (auto db : referenceLevels)
        m.addItem ("0 VU = " + juce::String (int (db)) + " dBFS", true, std::abs (db - current) < 0.01f, [this, db] { proc.setReferenceDb (db); });
}

void PluginEditor::addMenuItems (juce::PopupMenu& m)
{
    juce::PopupMenu sub;
    addReferenceItems (sub);
    m.addSubMenu ("Reference Level", sub);
}

void PluginEditor::showReferenceMenu()
{
    juce::PopupMenu m;
    m.addSectionHeader ("Reference Level");
    addReferenceItems (m);
    m.setLookAndFeel (&getLookAndFeel());
    m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this).withMousePosition());
}

void PluginEditor::updateCaptions()
{
    const auto caption = "0 VU = " + juce::String (int (proc.getReferenceDb())) + " dBFS";
    meterL.setCaption (caption);
    meterR.setCaption (caption);
}

//==============================================================================
int PluginEditor::meterWidthForWidth (int w) const
{
    return (w - 4 * inset - meterGap) / 2;
}

int PluginEditor::heightForWidth (int w) const
{
    return headerHeight + 4 * inset + juce::roundToInt (float (meterWidthForWidth (w)) / VUMeter::faceAspect);
}

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    // Keep the window exactly as tall as the meters need, whatever the host or user asks for
    if (const int wanted = heightForWidth (getWidth()); wanted != getHeight())
    {
        setSize (getWidth(), wanted);
        return;
    }

    auto meters = juce::Rectangle<int> (2 * inset, headerHeight + 2 * inset, getWidth() - 4 * inset, getHeight() - headerHeight - 4 * inset);
    const int meterW = meterWidthForWidth (getWidth());

    meterL.setBounds (meters.removeFromLeft (meterW));
    meters.removeFromLeft (meterGap);
    meterR.setBounds (meters);
}
