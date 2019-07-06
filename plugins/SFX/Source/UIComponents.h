/*
  ==============================================================================

    UIComponents.h
    Created: 5 Jul 2019 11:45:25am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"

//==============================================================================
class PadComponent : public Component,
                     private Timer
{
public:
    PadComponent (SFXAudioProcessor& pc, Pad& p)
        : processor (pc), pad (p)
    {
        startTimerHz (30);
    }

    std::function<void ()> onDown, onUp;

private:
    void timerCallback() override
    {
        bool newBright = processor.isMidiNoteDown (pad.note);
        if (newBright != bright)
        {
            bright = newBright;
            repaint();
        }
    }

    void mouseDown (const MouseEvent&) override
    {
        if (onDown)
            onDown();
    }

    void mouseUp (const MouseEvent&) override
    {
        if (onUp)
            onUp();
    }

    void paint (Graphics& g) override
    {
        auto rc = getLocalBounds().reduced (4);

        if (bright)
        {
            g.setColour (Colours::white.withAlpha (0.3f));
            g.fillRect (rc);
        }

        g.setColour (Colours::white);
        g.drawRect (rc);
    }

    SFXAudioProcessor& processor;
    Pad& pad;

    bool bright = false;
};

//==============================================================================
class PadGridComponent : public Component
{
public:
    PadGridComponent (SFXAudioProcessor& p)
        : processor (p)
    {
        for (auto p : processor.getPads())
        {
            auto pc = new PadComponent (processor, *p);
            addAndMakeVisible (pc);
            pads.add (pc);
        }
    }

    const OwnedArray<PadComponent>& getPads() { return pads; }

private:
    void resized() override
    {
        auto rc = getLocalBounds();

        int w = getWidth() / 4;
        int h = getHeight() / 4;

        for (int y = 0; y < 4; y++)
        {
            auto row = rc.removeFromTop (h);
            for (int x = 0; x < 4; x++)
            {
                pads[y * 4 + x]->setBounds (row.removeFromLeft (w));
            }
        }
    }

    SFXAudioProcessor& processor;

    OwnedArray<PadComponent> pads;
};

//==============================================================================
class ParamPageComponent : public Component
{
public:
    ParamPageComponent (Pad& p)
        : pad (p)
    {
        for (auto pp : pad.pluginParams)
        {
            auto pc = new gin::HorizontalFader (pp, false);
            addAndMakeVisible (pc);
            controls.add (pc);
        }
    }

private:
    void resized() override
    {
        auto rc1 = getLocalBounds();
        auto rc2 = rc1.removeFromRight (rc1.getWidth() / 2);

        for (auto c : controls)
        {
            if (rc1.getHeight() >= 20)
                c->setBounds (rc1.removeFromTop (20));
            else
                c->setBounds (rc2.removeFromTop (20));
        }
    }

    Pad& pad;
    OwnedArray<gin::ParamComponent> controls;
};

//==============================================================================
class ParamComponent : public Component
{
public:
    ParamComponent (SFXAudioProcessor& p)
        : processor (p)
    {
        for (auto p : processor.getPads())
        {
            auto ppc = new ParamPageComponent (*p);
            addChildComponent (ppc);

            pages.add (ppc);
        }

        setPage (0);
    }

    void setPage (int num)
    {
        for (auto p : pages)
            p->setVisible (false);

        pages[num]->setVisible (true);
    }

private:
    void resized() override
    {
        for (auto p : pages)
            p->setBounds (getLocalBounds());
    }

    SFXAudioProcessor& processor;
    OwnedArray<ParamPageComponent> pages;
};

