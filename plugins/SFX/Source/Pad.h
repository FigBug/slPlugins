/*
  ==============================================================================

    Pad.h
    Created: 4 Jul 2019 8:25:11pm
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Pad
{
public:
    Pad (int n)
        : note (n)
    {
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
        for (int i = 0; i < ids.size(); i++)
        {
            pluginParams[i]->setUserValueAsUserAction (params.getParam (ids[i]));
            pluginLockParams[i]->setUserValueAsUserAction (params.lockedParam (ids[i]) ? 1.0f : 0.0f);
        }
    }

    void fromPluginParams()
    {
        auto ids = params.getParams();
        for (int i = 0; i < ids.size(); i++)
        {
            params.setParam (ids[i], pluginParams[i]->getUserValue());
            params.setParamLocked (ids[i], pluginLockParams[i]->getUserValue() > 0.0f);
        }
    }

    String name;
    int note = 0;
    SfxrParams params;

    Array<gin::Parameter*> pluginParams, pluginLockParams;
};
