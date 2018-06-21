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
    envelopeBuffer.setSize (1, 128);
}

void FormulaVoice::updateParams()
{
    adsr.setAttack (owner.params.attack);
    adsr.setDecay (owner.params.decay);
    adsr.setSustainLevel (owner.params.sustain);
    adsr.setRelease (owner.params.release);
}

void FormulaVoice::setFormulas (String f1, String f2, String f3)
{
    oscillators[0].setFormula (f1);
    oscillators[1].setFormula (f2);
    oscillators[2].setFormula (f3);
}

void FormulaVoice::noteStarted()
{
    updateParams();
    
    adsr.noteOn();
    
    for (auto& o : oscillators)
        o.start();
}

void FormulaVoice::noteStopped (bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        adsr.reset();
        clearCurrentNote();
    }
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
    if (numSamples > envelopeBuffer.getNumSamples())
        envelopeBuffer.setSize (1, numSamples, false, false, true);
    
    samplesProcessed += numSamples;
    if (samplesProcessed >= 32)
        updateParams();
    
    auto note = getCurrentlyPlayingNote();
    double freq = note.getFrequencyInHertz();

    float* env = envelopeBuffer.getWritePointer (0);
    
    for (int i = 0; i < numSamples; i++)
        *env++ = adsr.process();
    
    int idx = 0;
    
    auto enabled = [&] (int i)
    {
        if (idx == 0) return owner.params.osc1enable;
        if (idx == 1) return owner.params.osc2enable;
        if (idx == 2) return owner.params.osc3enable;
        jassertfalse;
        return false;
    };
    
    for (auto& o : oscillators)
    {
        if (enabled (idx))
        {
            o.setGain (note.noteOnVelocity.asUnsignedFloat());
            o.setFrequency (freq);
            o.process (envelopeBuffer, outputBuffer, startSample, numSamples);
        }
        idx++;
    }
    
    if (adsr.getState() == ADSR::idle)
        clearCurrentNote();
}

