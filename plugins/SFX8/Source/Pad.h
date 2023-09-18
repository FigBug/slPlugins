/*
  ==============================================================================

    Pad.h
    Created: 4 Jul 2019 8:25:11pm
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../../modules/bfxr/PinkNumber.h"
#include "../../../modules/bfxr/SfxrParams.h"
#include "../../../modules/bfxr/SfxrSynth.h"

class Pad
{
public:
    Pad (juce::ValueTree& state, int i, int n)
        : index (i)
    {
        name.referTo (state, juce::String::formatted ("name%d", index), nullptr);
        note.referTo (state, juce::String::formatted ("note%d", index), nullptr);
        
        note = n;

        switch (rand() % 7)
        {
            case 0:
                params.generatePickupCoin();
                name = "Coin";
                break;
            case 1:
                params.generateLaserShoot();
                name = "Laser";
                break;
            case 2:
                params.generateExplosion();
                name = "Explosion";
                break;
            case 3:
                params.generatePowerup();
                name = "Powerup";
                break;
            case 4:
                params.generateHitHurt();
                name = "Hit";
                break;
            case 5:
                params.generateJump();
                name = "Jump";
                break;
            case 6:
                params.generateBlipSelect();
                name = "Coin";
                break;
        }
    }

    void toPluginParams()
    {
        auto ids = params.getParams();
        for (size_t i = 0; i < ids.size(); i++)
        {
            pluginParams[int (i)]->setUserValueAsUserAction (params.getParam (ids[i]));
            pluginLockParams[int (i)]->setUserValueAsUserAction (params.lockedParam (ids[i]) ? 1.0f : 0.0f);
        }
    }

    void fromPluginParams()
    {
        auto ids = params.getParams();
        for (size_t i = 0; i < ids.size(); i++)
        {
            params.setParam (ids[i], pluginParams[int (i)]->getUserValue());
            params.setParamLocked (ids[i], pluginLockParams[int (i)]->getUserValue() > 0.0f);
        }
    }

    int index = 0;
    juce::CachedValue<juce::String> name;
    juce::CachedValue<int> note;
    SfxrParams params;

    juce::Array<gin::Parameter*> pluginParams, pluginLockParams;
};
