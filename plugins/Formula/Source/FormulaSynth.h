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
        bool osc1enable = false, osc2enable = false, osc3enable = false;
        
        float ampAttack = 0, ampDecay = 0, ampSustain = 0, ampRelease = 0, ampVelocity = 0;
        float filterAttack = 0, filterDecay = 0, filterSustain = 0, filterRelease = 0;
        float filterFreq = 0, filterRes = 0, filterAmount = 0, filterKey = 0, filterVelocity = 0;
    };
    
    FormulaSynth();
    
    void setMPE (bool mpe);
    void setCurrentPlaybackSampleRate (double sampleRate) override;
    
    Params& getParams()     { return params; }
    
    void setFormulas (String f1, String f2, String f3);
    
private:
    friend FormulaVoice;
    
    void handleController (int /*midiChannel*/, int /*controllerNumber*/, int /*controllerValue*/) override;
    
    Params params;
    
    std::unique_ptr<gin::BandLimitedLookupTables> lookupTables;
};
