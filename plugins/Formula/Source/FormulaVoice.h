/*
  ==============================================================================

    FormulaVoice.h
    Created: 7 Jun 2018 8:56:38am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Oscillator.h"

using namespace gin;

class FormulaSynth;
class FormulaVoice : public MPESynthesiserVoice
{
public:
    FormulaVoice (FormulaSynth& owner);
    
    void setFormulas (String f1, String f2, String f3);
    
    void noteStarted() override;
    void noteStopped (bool allowTailOff) override;
    void notePressureChanged() override;
    void notePitchbendChanged() override;
    void noteTimbreChanged() override;
    void noteKeyStateChanged() override;
    
    void setController (int midiChannel, int controllerNumber, int controllerValue);
    
    void setCurrentSampleRate (double newRate) override;
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    void updateParams();
    
    FormulaSynth& owner;
    
    Oscillator oscillators[3];
    gin::ADSR adsr;
    
    AudioSampleBuffer envelopeBuffer;
    
    int samplesProcessed = 0;
};
