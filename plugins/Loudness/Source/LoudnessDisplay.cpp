#include "LoudnessDisplay.h"

//==============================================================================
LoudnessDisplay::LoudnessDisplay (PluginProcessor& p)
    : proc (p)
{
    setOpaque (false);

    addAndMakeVisible (resetButton);
    resetButton.onClick = [this] { proc.resetMeters(); };

    startTimerHz (30);
}

LoudnessDisplay::~LoudnessDisplay()
{
    stopTimer();
}

void LoudnessDisplay::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu() && onContextMenu)
        onContextMenu();
}

void LoudnessDisplay::resized()
{
    auto rc = getLocalBounds().reduced (8);
    readoutArea = rc.removeFromRight (juce::jlimit (150, 220, rc.getWidth() / 3));
    rc.removeFromRight (12);
    barsArea = rc;

    resetButton.setBounds (readoutArea.removeFromBottom (26).withSizeKeepingCentre (juce::jmin (90, readoutArea.getWidth()), 26));
    readoutArea.removeFromBottom (8);
}

//==============================================================================
LoudnessDisplay::ScaleInfo LoudnessDisplay::scaleInfo() const
{
    switch (proc.getScale())
    {
        case PluginProcessor::k20: return { -40.0f, 20.0f, 4.0f, 20.0f, 3.01f, 0.0f, 4.0f, "K-20" };
        case PluginProcessor::k14: return { -40.0f, 14.0f, 4.0f, 14.0f, 3.01f, 0.0f, 4.0f, "K-14" };
        case PluginProcessor::k12: return { -40.0f, 12.0f, 4.0f, 12.0f, 3.01f, 0.0f, 4.0f, "K-12" };
        default:                   return { -60.0f,  3.0f, 6.0f,  0.0f, 0.0f, -18.0f, -6.0f, "dBFS" };
    }
}

juce::String LoudnessDisplay::formatDb (float v, const juce::String& unit, int decimals)
{
    if (v <= -99.0f)
        return "-inf " + unit;

    if (std::abs (v) < 0.05f)
        v = 0.0f;   // avoid printing -0.0

    return juce::String (v, decimals) + " " + unit;
}

float LoudnessDisplay::valueToY (float v, float bottom, float top, juce::Rectangle<float> bar)
{
    const float t = juce::jlimit (0.0f, 1.0f, (v - bottom) / (top - bottom));
    return bar.getBottom() - t * bar.getHeight();
}

//==============================================================================
void LoudnessDisplay::timerCallback()
{
    const float fall = 24.0f / 30.0f;   // 24 dB per second at 30 Hz

    for (int ch = 0; ch < 2; ch++)
    {
        rmsDb[ch]  = proc.rms.getRMS (ch);
        holdDb[ch] = proc.truePeak.getMaxTruePeak (ch);

        const float p = proc.truePeak.getTruePeak (ch);
        peakDb[ch] = p > peakDb[ch] ? p : juce::jmax (p, peakDb[ch] - fall);
    }

    momentary = proc.loudness.getMomentary();
    shortTerm = proc.loudness.getShortTerm();
    maxPeak   = proc.truePeak.getMaxTruePeak();
    elapsed   = proc.loudness.getElapsedSeconds();

    // The gated measurements walk the whole history, so refresh them at 2 Hz
    if (++slowTick >= 15)
    {
        slowTick = 0;
        integrated = proc.loudness.getIntegrated();
        range      = proc.loudness.getLoudnessRange();
    }

    repaint();
}

//==============================================================================
void LoudnessDisplay::drawScale (juce::Graphics& g, juce::Rectangle<float> bar, float bottom, float top, float step, bool labelsOnLeft)
{
    g.setFont (juce::Font (juce::FontOptions (10.0f)));

    for (float v = bottom; v <= top + 0.01f; v += step)
    {
        const float y = valueToY (v, bottom, top, bar);
        g.setColour (findColour (gin::PluginLookAndFeel::grey60ColourId));

        if (labelsOnLeft)
        {
            g.fillRect (juce::Rectangle<float> (bar.getX() - 5.0f, y - 0.5f, 4.0f, 1.0f));
            g.drawText (juce::String (int (v)), juce::Rectangle<float> (bar.getX() - 36.0f, y - 7.0f, 29.0f, 14.0f), juce::Justification::centredRight, false);
        }
        else
        {
            g.fillRect (juce::Rectangle<float> (bar.getRight() + 1.0f, y - 0.5f, 4.0f, 1.0f));
            g.drawText (juce::String (int (v)), juce::Rectangle<float> (bar.getRight() + 7.0f, y - 7.0f, 29.0f, 14.0f), juce::Justification::centredLeft, false);
        }
    }
}

void LoudnessDisplay::drawLevelBar (juce::Graphics& g, juce::Rectangle<float> bar, const ScaleInfo& s, float rms, float peak, float hold)
{
    const auto track  = findColour (gin::PluginLookAndFeel::grey30ColourId);
    const auto green  = findColour (gin::PluginLookAndFeel::accentColourId);
    const auto yellow = juce::Colour (0xffe0b23c);
    const auto red    = juce::Colour (0xffd92b2b);

    g.setColour (track);
    g.fillRect (bar);

    const float rmsShown  = rms  + s.offset + s.rmsOffset;
    const float peakShown = peak + s.offset;
    const float holdShown = hold + s.offset;

    // RMS fill, split into colour zones
    const float yRms = valueToY (rmsShown, s.bottom, s.top, bar);
    const float yYellow = valueToY (s.yellow, s.bottom, s.top, bar);
    const float yRed    = valueToY (s.red,    s.bottom, s.top, bar);

    auto fillZone = [&] (float yTop, float yBottom, juce::Colour c)
    {
        if (yTop < yBottom)
        {
            g.setColour (c);
            g.fillRect (juce::Rectangle<float> (bar.getX(), yTop, bar.getWidth(), yBottom - yTop));
        }
    };

    fillZone (juce::jmax (yRms, yYellow), bar.getBottom(), green);
    fillZone (juce::jmax (yRms, yRed), juce::jmin (bar.getBottom(), yYellow), yellow);
    fillZone (yRms, juce::jmin (bar.getBottom(), yRed), red);

    // Peak as a short line, max as a thinner held line
    if (peak > -99.0f)
    {
        const float y = valueToY (peakShown, s.bottom, s.top, bar);
        g.setColour (peakShown >= s.red ? red : (peakShown >= s.yellow ? yellow : green));
        g.fillRect (juce::Rectangle<float> (bar.getX(), y - 1.5f, bar.getWidth(), 3.0f));
    }

    if (hold > -99.0f)
    {
        const float y = valueToY (holdShown, s.bottom, s.top, bar);
        g.setColour (findColour (gin::PluginLookAndFeel::whiteColourId));
        g.fillRect (juce::Rectangle<float> (bar.getX(), y - 0.5f, bar.getWidth(), 1.0f));
    }
}

void LoudnessDisplay::drawLoudnessBar (juce::Graphics& g, juce::Rectangle<float> bar, float m, float st)
{
    const float bottom = -60.0f, top = 0.0f;

    g.setColour (findColour (gin::PluginLookAndFeel::grey30ColourId));
    g.fillRect (bar);

    if (m > -99.0f)
    {
        const float y = valueToY (m, bottom, top, bar);
        g.setColour (findColour (gin::PluginLookAndFeel::accentColourId));
        g.fillRect (juce::Rectangle<float> (bar.getX(), y, bar.getWidth(), bar.getBottom() - y));
    }

    if (st > -99.0f)
    {
        const float y = valueToY (st, bottom, top, bar);
        g.setColour (findColour (gin::PluginLookAndFeel::whiteColourId));
        g.fillRect (juce::Rectangle<float> (bar.getX(), y - 0.5f, bar.getWidth(), 1.0f));
    }
}

void LoudnessDisplay::paint (juce::Graphics& g)
{
    const auto s = scaleInfo();
    const auto text  = findColour (gin::PluginLookAndFeel::whiteColourId);
    const auto muted = findColour (gin::PluginLookAndFeel::grey60ColourId);

    //==============================================================================
    // Bars: scale | L | R | gap | M | scale
    {
        auto rc = barsArea.toFloat();
        auto labels = rc.removeFromBottom (16.0f);
        rc.removeFromBottom (4.0f);
        rc.removeFromTop (8.0f);

        const float scaleW = 36.0f;
        rc.removeFromLeft (scaleW);
        rc.removeFromRight (scaleW);

        const float gap = 6.0f;
        const float barW = juce::jmax (8.0f, (rc.getWidth() - gap * 2.0f - 16.0f) / 3.0f);

        auto left  = rc.removeFromLeft (barW); rc.removeFromLeft (gap);
        auto right = rc.removeFromLeft (barW); rc.removeFromLeft (gap + 16.0f);
        auto mom   = rc.removeFromLeft (barW);

        drawScale (g, left, s.bottom, s.top, s.step, true);
        drawLevelBar (g, left,  s, rmsDb[0], peakDb[0], holdDb[0]);
        drawLevelBar (g, right, s, rmsDb[1], peakDb[1], holdDb[1]);

        drawLoudnessBar (g, mom, momentary, shortTerm);
        drawScale (g, mom, -60.0f, 0.0f, 6.0f, false);

        g.setColour (muted);
        g.setFont (juce::Font (juce::FontOptions (11.0f)));
        g.drawText ("L", left.withY (labels.getY()).withHeight (labels.getHeight()), juce::Justification::centred, false);
        g.drawText ("R", right.withY (labels.getY()).withHeight (labels.getHeight()), juce::Justification::centred, false);
        g.drawText ("M", mom.withY (labels.getY()).withHeight (labels.getHeight()), juce::Justification::centred, false);
    }

    //==============================================================================
    // Readouts
    {
        auto rc = readoutArea.toFloat();
        const float rowH = juce::jlimit (22.0f, 34.0f, rc.getHeight() / 7.0f);

        auto row = [&] (const juce::String& label, const juce::String& value)
        {
            auto r = rc.removeFromTop (rowH);
            g.setColour (muted);
            g.setFont (juce::Font (juce::FontOptions (10.0f)));
            g.drawText (label, r.removeFromTop (r.getHeight() * 0.42f), juce::Justification::centredLeft, false);
            g.setColour (text);
            g.setFont (juce::Font (juce::FontOptions (juce::jmin (16.0f, r.getHeight()), juce::Font::bold)));
            g.drawText (value, r, juce::Justification::centredLeft, false);
        };

        const int secs = int (elapsed);
        const auto time = juce::String::formatted ("%d:%02d:%02d", secs / 3600, (secs / 60) % 60, secs % 60);

        row ("MOMENTARY",  formatDb (momentary, "LUFS"));
        row ("SHORT TERM", formatDb (shortTerm, "LUFS"));
        row ("INTEGRATED", formatDb (integrated, "LUFS"));
        row ("RANGE",      juce::String (range, 1) + " LU");
        row ("TRUE PEAK",  formatDb (maxPeak, "dBTP"));
        row ("SCALE",      s.name + "   " + time);
    }
}
