#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PitchScope.h"

//==============================================================================
class ColourSwatch : public juce::Component,
                     public juce::TooltipClient,
                     private juce::ChangeListener
{
public:
    ColourSwatch() { setMouseCursor (juce::MouseCursor::PointingHandCursor); }

    void setColour (juce::Colour c) { colour = c; repaint(); }
    juce::Colour getColour() const { return colour; }
    void setTooltip (const juce::String& t) { tooltip = t; }
    juce::String getTooltip() override { return tooltip; }

    void paint (juce::Graphics& g) override
    {
        g.setColour (colour);
        g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (2), 3.0f);
        g.setColour (juce::Colours::white.withAlpha (0.5f));
        g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (2), 3.0f, 1.0f);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        if (e.mods.isShiftDown())
        {
            colour = defaultColour;
            repaint();
            if (onColourChanged)
                onColourChanged (colour);
            return;
        }

        auto colourSelector = std::make_unique<juce::ColourSelector> (
            juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
        colourSelector->setCurrentColour (colour);
        colourSelector->setSize (300, 400);
        colourSelector->addChangeListener (this);

        auto& cb = juce::CallOutBox::launchAsynchronously (std::move (colourSelector), getScreenBounds(), nullptr);
        cb.setDismissalMouseClicksAreAlwaysConsumed (true);
    }

    void setDefaultColour (juce::Colour c) { defaultColour = c; }

    std::function<void (juce::Colour)> onColourChanged;

private:
    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (auto* cs = dynamic_cast<juce::ColourSelector*> (source))
        {
            colour = cs->getCurrentColour();
            repaint();
            if (onColourChanged)
                onColourChanged (colour);
        }
    }

    juce::Colour colour { juce::Colours::white };
    juce::Colour defaultColour { juce::Colours::white };
    juce::String tooltip;
};

//==============================================================================
/**
*/
class PluginEditor  : public gin::ProcessorEditor,
                      private gin::Parameter::ParameterListener,
                      private juce::Timer
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void resized() override;
    juce::Rectangle<int> getGridArea (int x, int y, int w = 1, int h = 1) override;

private:
    juce::CriticalSection lock;
    void updateScope();
    void updateTraceColours();
    void valueUpdated (gin::Parameter*) override { updateScope(); }

    void timerCallback() override;
    void updateRecordButtons();
    void updateRecordButtonColours();
    void updatePlayPauseButton();
    void showRecordMenu();
    void setFullscreen (bool fs);

    PluginProcessor& scopeProc;

    PitchScope scope { scopeProc.fifo, scopeProc };

    gin::SVGButton playPauseButton;
    gin::SVGButton fullscreenButton;
    gin::SVGButton recordNormal;
    gin::SVGButton recordTriggered;
    gin::SVGButton recordRetrospective;
    gin::SVGButton recordStop;
    gin::SVGButton recordMenu;
    ColourSwatch colourSwatch1;
    ColourSwatch colourSwatch2;
    bool flashState = false;
    bool isFullscreen = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
