#include "CorrelationDisplay.h"

//==============================================================================
CorrelationDisplay::CorrelationDisplay (const gin::CorrelationMeter& m)
    : meter (m)
{
    setOpaque (false);
    startTimerHz (30);
}

CorrelationDisplay::~CorrelationDisplay()
{
    stopTimer();
}

void CorrelationDisplay::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu() && onContextMenu)
        onContextMenu();
}

void CorrelationDisplay::timerCallback()
{
    const float c = meter.getCorrelation();
    const float b = meter.getBalance();

    if (std::abs (c - correlation) > 0.002f || std::abs (b - balance) > 0.002f)
    {
        correlation = c;
        balance = b;
        repaint();
    }
}

//==============================================================================
void CorrelationDisplay::paint (juce::Graphics& g)
{
    const auto text   = findColour (gin::PluginLookAndFeel::whiteColourId);
    const auto muted  = findColour (gin::PluginLookAndFeel::grey60ColourId);
    const auto track  = findColour (gin::PluginLookAndFeel::grey30ColourId);
    const auto accent = findColour (gin::PluginLookAndFeel::accentColourId);
    const auto red    = juce::Colour (0xffd92b2b);

    auto area = getLocalBounds().toFloat().reduced (12.0f, 8.0f);
    const float labelW = 34.0f;

    // Correlation: label, then a bar that fills outwards from the centre
    {
        auto row = area.removeFromTop (area.getHeight() * 0.55f);

        g.setColour (muted);
        g.setFont (juce::Font (juce::FontOptions (13.0f)));
        g.drawText ("-1", row.removeFromLeft (labelW), juce::Justification::centredLeft, false);
        g.drawText ("+1", row.removeFromRight (labelW), juce::Justification::centredRight, false);

        auto bar = row.withSizeKeepingCentre (row.getWidth(), juce::jmin (row.getHeight(), 22.0f));
        const float r = bar.getHeight() * 0.5f;
        const float centre = bar.getCentreX();

        g.setColour (track);
        g.fillRoundedRectangle (bar, r);

        const float x = centre + correlation * bar.getWidth() * 0.5f;
        auto fill = correlation >= 0.0f ? juce::Rectangle<float> (centre, bar.getY(), x - centre, bar.getHeight())
                                        : juce::Rectangle<float> (x, bar.getY(), centre - x, bar.getHeight());

        g.saveState();
        juce::Path clip;
        clip.addRoundedRectangle (bar, r);
        g.reduceClipRegion (clip);
        g.setColour (correlation >= 0.0f ? accent : red);
        g.fillRect (fill);
        g.restoreState();

        // Tick marks at -0.5, 0, +0.5
        g.setColour (text.withAlpha (0.6f));
        for (float t : { -0.5f, 0.0f, 0.5f })
        {
            const float tx = centre + t * bar.getWidth() * 0.5f;
            g.fillRect (juce::Rectangle<float> (tx - 0.75f, bar.getY() - 4.0f, 1.5f, 4.0f));
            g.fillRect (juce::Rectangle<float> (tx - 0.75f, bar.getBottom(), 1.5f, 4.0f));
        }

        g.setColour (text);
        g.setFont (juce::Font (juce::FontOptions (13.0f, juce::Font::bold)));
        g.drawText (juce::String (correlation >= 0.0f ? "+" : "") + juce::String (correlation, 2),
                    bar, juce::Justification::centred, false);
    }

    area.removeFromTop (6.0f);

    // Balance: thin track with a marker, L and R at the ends
    {
        auto row = area;

        g.setColour (muted);
        g.setFont (juce::Font (juce::FontOptions (13.0f)));
        g.drawText ("L", row.removeFromLeft (labelW), juce::Justification::centredLeft, false);
        g.drawText ("R", row.removeFromRight (labelW), juce::Justification::centredRight, false);

        auto bar = row.withSizeKeepingCentre (row.getWidth(), 4.0f);
        g.setColour (track);
        g.fillRoundedRectangle (bar, 2.0f);

        g.setColour (text.withAlpha (0.6f));
        g.fillRect (juce::Rectangle<float> (bar.getCentreX() - 0.75f, bar.getY() - 5.0f, 1.5f, bar.getHeight() + 10.0f));

        const float x = bar.getCentreX() + balance * bar.getWidth() * 0.5f;
        g.setColour (accent);
        g.fillEllipse (juce::Rectangle<float> (12.0f, 12.0f).withCentre ({ x, bar.getCentreY() }));
    }
}
