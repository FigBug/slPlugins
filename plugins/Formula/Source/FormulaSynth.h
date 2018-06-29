/*
  ==============================================================================

    FormulaSynth.h
    Created: 7 Jun 2018 8:56:26am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FormulaVoice;
class FormulaSynth : public MPESynthesiser
{
public:
    struct Params
    {
        bool osc1enable = false;
        bool osc2enable = false;
        bool osc3enable = false;
        
        float attack  = 0;
        float decay   = 0;
        float sustain = 0;
        float release = 0;
    };
    
    FormulaSynth();
    
    void setMPE (bool mpe);
    
    Params& getParams()     { return params; }
    
    void setFormulas (String f1, String f2, String f3);
    
private:
    friend FormulaVoice;
    
    void handleController (int /*midiChannel*/, int /*controllerNumber*/, int /*controllerValue*/) override;
    
    Params params;
};
