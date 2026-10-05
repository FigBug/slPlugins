#include "VUMeter.h"

//==============================================================================
namespace
{
    const juce::Colour faceColour (0xfff3f0ea);
    const juce::Colour inkColour  (0xff2b2724);
    const juce::Colour redColour  (0xffd92b2b);
}

VUMeter::VUMeter (std::atomic<float>& level, std::atomic<bool>& peak, const juce::String& name)
    : levelSource (level), peakSource (peak), channelName (name)
{
    setOpaque (false);
    startTimerHz (60);
}

VUMeter::~VUMeter()
{
    stopTimer();
}

void VUMeter::setCaption (const juce::String& c)
{
    if (caption != c)
    {
        caption = c;
        repaint();
    }
}

void VUMeter::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu() && onContextMenu)
        onContextMenu();
}

//==============================================================================
void VUMeter::timerCallback()
{
    bool dirty = false;

    const float newLevel = levelSource.load (std::memory_order_relaxed);
    if (std::abs (newLevel - needleLinear) > 0.0005f)
    {
        needleLinear = newLevel;
        dirty = true;
    }

    const auto now = juce::Time::getMillisecondCounter();

    if (peakSource.exchange (false, std::memory_order_relaxed))
    {
        peakLitUntil = now + 1500;
        dirty = dirty || ! peakLit;
        peakLit = true;
    }
    else if (peakLit && now > peakLitUntil)
    {
        peakLit = false;
        dirty = true;
    }

    if (dirty)
        repaint();
}

//==============================================================================
void VUMeter::paint (juce::Graphics& g)
{
    // Largest face of the right aspect that fits, centred
    auto face = getLocalBounds().toFloat();
    if (face.getWidth() / face.getHeight() > faceAspect)
        face = face.withSizeKeepingCentre (face.getHeight() * faceAspect, face.getHeight());
    else
        face = face.withSizeKeepingCentre (face.getWidth(), face.getWidth() / faceAspect);

    if (face.isEmpty())
        return;

    const float h = face.getHeight();
    const juce::Point<float> pivot (face.getCentreX(), face.getY() + 1.3f * h);   // hidden below the frame
    const float R = 0.95f * h;

    auto pointAt = [&] (float pos, float radius)
    {
        const float a = positionToAngle (pos);
        return juce::Point<float> (pivot.x + radius * std::sin (a), pivot.y - radius * std::cos (a));
    };

    g.setColour (faceColour);
    g.fillRoundedRectangle (face, 0.04f * h);

    juce::Path clip;
    clip.addRoundedRectangle (face, 0.04f * h);
    g.reduceClipRegion (clip);

    // Scale arc: dark up to 0 VU, red beyond
    {
        const float zero = linearToPosition (1.0f);
        const float lineW = 0.012f * h;

        juce::Path dark, red;
        dark.addCentredArc (pivot.x, pivot.y, R, R, 0.0f, positionToAngle (0.0f), positionToAngle (zero), true);
        red.addCentredArc  (pivot.x, pivot.y, R, R, 0.0f, positionToAngle (zero), positionToAngle (1.0f), true);

        g.setColour (inkColour);
        g.strokePath (dark, juce::PathStrokeType (lineW));
        g.setColour (redColour);
        g.strokePath (red, juce::PathStrokeType (lineW));
    }

    // Ticks and numbers
    {
        struct Mark { float db; const char* label; };
        const Mark marks[] = { { -20, "20" }, { -10, "10" }, { -7, "7" }, { -5, "5" }, { -3, "3" }, { -2, "2" },
                               {  -1, "1" },  {   0, "0" },  {  1, "1" }, {  2, "2" }, {  3, "3" } };

        const float tickLen = 0.06f * h;
        g.setFont (juce::Font (juce::FontOptions (0.1f * h)));

        for (auto& m : marks)
        {
            const float pos = linearToPosition (juce::Decibels::decibelsToGain (m.db));
            const float a   = positionToAngle (pos);

            g.setColour (m.db > 0 ? redColour : inkColour);
            g.drawLine (juce::Line<float> (pointAt (pos, R), pointAt (pos, R + tickLen)), 0.012f * h);

            const auto p = pointAt (pos, R + tickLen + 0.08f * h);
            g.saveState();
            g.addTransform (juce::AffineTransform::rotation (a, p.x, p.y));
            g.drawText (m.label, juce::Rectangle<float> (0.4f * h, 0.14f * h).withCentre (p), juce::Justification::centred, false);
            g.restoreState();
        }
    }

    // Legends
    g.setColour (inkColour.withAlpha (0.6f));
    g.setFont (juce::Font (juce::FontOptions (0.16f * h, juce::Font::bold)));
    g.drawText ("VU", face.withTrimmedTop (0.66f * h), juce::Justification::centredTop, false);

    g.setFont (juce::Font (juce::FontOptions (0.09f * h)));
    g.drawText (channelName, face.reduced (0.06f * h), juce::Justification::bottomLeft, false);

    g.setFont (juce::Font (juce::FontOptions (0.07f * h)));
    g.drawText (caption, face.reduced (0.06f * h), juce::Justification::centredBottom, false);

    // Peak dot
    {
        const auto dot = juce::Rectangle<float> (0.07f * h, 0.07f * h).withCentre ({ face.getRight() - 0.1f * h, face.getBottom() - 0.1f * h });

        if (peakLit)
        {
            g.setColour (redColour);
            g.fillEllipse (dot);
        }
        else
        {
            g.setColour (inkColour.withAlpha (0.25f));
            g.drawEllipse (dot, juce::jmax (1.0f, 0.008f * h));
        }
    }

    // Needle, with a little travel past both ends like a real movement
    {
        const float pos = juce::jlimit (-0.04f, 1.08f, linearToPosition (needleLinear));
        g.setColour (redColour);
        g.drawLine (juce::Line<float> (pivot, pointAt (pos, R + 0.08f * h)), 0.012f * h);
    }
}
