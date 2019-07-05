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
class PadComponent : public Component
{
public:
    PadComponent (Pad& p)
        : pad (p)
    {
    }

private:
    void paint (Graphics& g) override
    {
        auto rc = getLocalBounds().reduced (4);
        g.setColour (Colours::white);
        g.drawRect (rc);
    }

    Pad& pad;
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
            auto pc = new PadComponent (*p);
            addAndMakeVisible (pc);
            pads.add (pc);
        }
    }

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

        if (auto f = pages.getFirst())
            f->setVisible (true);
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

