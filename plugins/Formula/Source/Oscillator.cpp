/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Jun 2018 11:03:19am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator()
{
}

Oscillator::~Oscillator()
{
}

void Oscillator::setSampleRate (double sr)
{
    jassert (sr > 8000);    
    sampleRate = sr;
    
    for (auto& c : controllers)
        c.reset (sr, 0.05);
    
    for (auto& itr : funcStates)
        itr.second->setSampleRate (sr);
    
    funcStates.clear();
}

void Oscillator::setFormula (String formula)
{
    std::unique_ptr<gin::EquationParser> p;
    
    {
        ScopedLock sl (lock);
        std::swap (parser, p);
    }
    
    jassert (sampleRate > 8000);
    
    funcStates.clear();
    
    p = std::make_unique<gin::EquationParser>();
    
    p->setEquation (formula);
    p->addVariable ("note", &note);
    p->addVariable ("freq", &frequency);
    p->addVariable ("env", &envelope);
    p->addVariable ("cutoff", &cutoff);
    p->addVariable ("res", &res);
    
    for (int i = 0; i <= 127; i++)
        p->addVariable (String::formatted ("cc%d", i), controllers[i].getValuePtr());
    
    addOscillatorFunctions (*p);
    addSynthFilterFunctions (*p);

    p->evaluate();
    
    {
        ScopedLock sl (lock);
        std::swap (parser, p);
    }
}

void Oscillator::start()
{
    for (auto& c : controllers)
        c.snapToValue();
    
    for (auto& itr : funcStates)
        itr.second->reset();
}

void Oscillator::process (AudioSampleBuffer& envelopeBuffer, AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    ScopedLock sl (lock);
    
    float* l = buffer.getWritePointer (0, startSample);
    float* r = buffer.getWritePointer (1, startSample);
    
    const float* e = envelopeBuffer.getReadPointer (0);
    
    if (parser != nullptr)
    {
        for (int i = 0; i < numSamples; i++)
        {
            for (auto& c : controllers)
                c.updateValue();
            
            envelope = e[i];
            
            auto out = parser->evaluate();
            
            *l++ += out * gain;
            *r++ += out * gain;
        }
    }
}
