/*
  ==============================================================================

    FormulaVoice.cpp
    Created: 7 Jun 2018 8:56:38am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "FormulaVoice.h"
#include "FormulaSynth.h"

FormulaVoice::FormulaVoice (FormulaSynth& o)
  : owner (o)
{
}

void FormulaVoice::noteStarted()
{
}

void FormulaVoice::noteStopped (bool allowTailOff)
{
}

void FormulaVoice::notePressureChanged()
{
}

void FormulaVoice::notePitchbendChanged()
{
}

void FormulaVoice::noteTimbreChanged()
{
}

void FormulaVoice::noteKeyStateChanged()
{
}

void FormulaVoice::setCurrentSampleRate (double newRate)
{
    MPESynthesiserVoice::setCurrentSampleRate (newRate);
    
    for (auto& o : oscillators)
        o.setSampleRate (newRate);
    
    adsr.setSampleRate (newRate);
}

void FormulaVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    outputBuffer.clear();
    
    for (auto& o : oscillators)
        o.process (outputBuffer, startSample, numSamples);
}

