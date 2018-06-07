/*
  ==============================================================================

    FormulaSynth.h
    Created: 7 Jun 2018 8:56:26am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FormulaSynth : public MPESynthesiser
{
public:
    FormulaSynth();
    
    void setMPE (bool mpe);
};
