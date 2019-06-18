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
    ampEnvelope.setAttack (owner.params.ampAttack);
    ampEnvelope.setDecay (owner.params.ampDecay);
    ampEnvelope.setSustainLevel (owner.params.ampSustain);
    ampEnvelope.setRelease (owner.params.ampRelease);
    
    filterEnvelope.setAttack (owner.params.filterAttack);
    filterEnvelope.setDecay (owner.params.filterDecay);
    filterEnvelope.setSustainLevel (owner.params.filterSustain);
    filterEnvelope.setRelease (owner.params.filterRelease);
    
    float filterEnv = filterEnvelope.getOutput();
    float filterSens = owner.params.filterVelocity / 100.0f;
    filterEnv *= currentlyPlayingNote.noteOnVelocity.asUnsignedFloat() * filterSens + 1.0f - filterSens;
    
    float freqNote = owner.params.filterFreq;
    freqNote += (currentlyPlayingNote.initialNote - 60) * owner.params.filterKey / 100.0f;
    freqNote += filterEnv * (owner.params.filterAmount * 135.076232f);
    
    float cutoff = jlimit (8.0f, jmin (20000.0f, float (currentSampleRate) / 2.0f), (float) getMidiNoteInHertz (freqNote));
    
    for (auto& o : oscillators)
        o.setCutoffRes (cutoff, owner.params.filterRes / 100.0f);
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
    
    ampEnvelope.noteOn();
    filterEnvelope.noteOn();
    
    for (auto& o : oscillators)
        o.start();
}

void FormulaVoice::noteStopped (bool allowTailOff)
{
    if (allowTailOff)
    {
        ampEnvelope.noteOff();
        filterEnvelope.noteOff();
    }
    else
    {
        ampEnvelope.reset();
        filterEnvelope.reset();
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
    {
        o.setSampleRate (newRate);
        o.lookupTables = owner.lookupTables.get();
    }
    
    ampEnvelope.setSampleRate (newRate);
    filterEnvelope.setSampleRate (newRate);
}

void FormulaVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (numSamples <= 0)
        return;
    
    if (numSamples > envelopeBuffer.getNumSamples())
        envelopeBuffer.setSize (1, numSamples, false, false, true);
    
    samplesProcessed += numSamples;
    if (samplesProcessed >= 32)
        updateParams();
    
    auto note = getCurrentlyPlayingNote();
    double noteNum = note.initialNote + note.totalPitchbendInSemitones;

    float* env = envelopeBuffer.getWritePointer (0);
    
    for (int i = 0; i < numSamples; i++)
    {
        *env++ = ampEnvelope.process();
        filterEnvelope.process();
    }
    
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
            auto gain = velocityToGain (note.noteOnVelocity.asUnsignedFloat(), owner.params.ampVelocity);
            o.setGain (gain);
            o.setNote (noteNum);
            o.process (envelopeBuffer, outputBuffer, startSample, numSamples);
        }
        idx++;
    }
    
    if (ampEnvelope.getState() == gin::AnalogADSR::State::idle)
        clearCurrentNote();
}

void FormulaVoice::setController (int midiChannel, int controllerNumber, int controllerValue)
{
    for (auto& o : oscillators)
        o.setController (controllerNumber, controllerValue / 127.0);
}
