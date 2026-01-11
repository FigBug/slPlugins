#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class LabelLookAndFeel : public gin::CopperLookAndFeel
{
public:
    juce::Font getLabelFont (juce::Label& label) override
    {
        return label.getFont();
    }
};

//==============================================================================
class CentsDisplay : public juce::Component,
                     private juce::Timer
{
public:
    CentsDisplay()
    {
        startTimerHz (60);
    }

    void setCents (float c)
    {
        targetCents = c;
    }

    void timerCallback() override
    {
        auto diff = targetCents - displayedCents;
        if (std::abs (diff) > 0.01f)
        {
            displayedCents += diff * damping;
            repaint();
        }
        else
        {
            displayedCents = targetCents;
            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        auto centreY = bounds.getCentreY();

        // Draw tick marks every 10 cents
        g.setColour (findColour (gin::CopperLookAndFeel::grey90ColourId).withAlpha (0.3f));
        for (int i = -50; i <= 50; i += 10)
        {
            auto normX = i / 50.0f;
            auto tickX = bounds.getCentreX() + normX * (bounds.getWidth() / 2.0f - 10.0f);
            auto tickHeight = (i == 0) ? bounds.getHeight() * 0.5f : bounds.getHeight() * 0.3f;
            g.drawLine (tickX, centreY - tickHeight / 2, tickX, centreY + tickHeight / 2, 1.0f);
        }

        g.setColour (findColour (gin::CopperLookAndFeel::grey90ColourId));

        if (std::abs (displayedCents) < 0.5f)
        {
            // Draw rounded vertical bar in the middle
            auto barWidth = 4.0f;
            auto barRect = bounds.withSizeKeepingCentre (barWidth, bounds.getHeight() * 0.8f);
            g.fillRoundedRectangle (barRect, barWidth / 2.0f);
        }
        else
        {
            // Map cents (-50..50) to x position
            auto normalisedX = juce::jlimit (-50.0f, 50.0f, displayedCents) / 50.0f; // -1..1
            auto centreX = bounds.getCentreX() + normalisedX * (bounds.getWidth() / 2.0f - 10.0f);

            auto arrowHeight = bounds.getHeight() * 0.6f;
            auto arrowWidth = 12.0f;

            juce::Path arrow;

            if (displayedCents < 0)
            {
                // Arrow pointing right (on left side)
                arrow.addTriangle (centreX - arrowWidth / 2, centreY - arrowHeight / 2,
                                   centreX - arrowWidth / 2, centreY + arrowHeight / 2,
                                   centreX + arrowWidth / 2, centreY);
            }
            else
            {
                // Arrow pointing left (on right side)
                arrow.addTriangle (centreX + arrowWidth / 2, centreY - arrowHeight / 2,
                                   centreX + arrowWidth / 2, centreY + arrowHeight / 2,
                                   centreX - arrowWidth / 2, centreY);
            }

            auto roundedArrow = arrow.createPathWithRoundedCorners (3.0f);
            g.fillPath (roundedArrow);
        }
    }

private:
    float targetCents = 0.0f;
    float displayedCents = 0.0f;
    float damping = 0.15f;
};

//==============================================================================
/**
*/
class PitchTrackAudioProcessorEditor : public gin::ProcessorEditor,
                                       private juce::Timer
{
public:
    PitchTrackAudioProcessorEditor (PitchTrackAudioProcessor&);
    ~PitchTrackAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint (juce::Graphics& g) override;
    void timerCallback() override;

private:
    PitchTrackAudioProcessor& proc;

    LabelLookAndFeel labelLookAndFeel;

    juce::Label noteLabel;
    juce::Label centsLabel;
    juce::Label pitchLabel;
    CentsDisplay centsDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchTrackAudioProcessorEditor)
};
