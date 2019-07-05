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
            case 0: params.generatePickupCoin(); break;
            case 1: params.generateLaserShoot(); break;
            case 2: params.generateExplosion();  break;
            case 3: params.generatePowerup();    break;
            case 4: params.generateHitHurt();    break;
            case 5: params.generateJump();       break;
            case 6: params.generateBlipSelect(); break;
        }
    }

    int note = 0;
    SfxrParams params;

    Array<gin::Parameter*> pluginParams;
};
