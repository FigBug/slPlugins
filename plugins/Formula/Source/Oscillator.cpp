/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Jun 2018 11:03:19am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "Oscillator.h"

static double sine (double phase)
{
    return std::sin (phase * 2 * MathConstants<double>::pi);
}

Oscillator::Oscillator()
{
}

Oscillator::~Oscillator()
{
}

void Oscillator::setFormula (String formula)
{
    auto* p = new gin::EquationParser();
    
    p->setEquation (formula);
    p->addVariable ("phase", &phase);
    p->addVariable ("freq", &frequency);
    p->addVariable ("env", &envelope);
    
    p->addFunction ("sine", sine);
    
    ScopedLock sl (lock);
    parser = p;
}

void Oscillator::start()
{
    phase = 0;
}

void Oscillator::process (AudioSampleBuffer& envelopeBuffer, AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    ScopedLock sl (lock);
    
    float* l = buffer.getWritePointer (0, startSample);
    float* r = buffer.getWritePointer (1, startSample);
    
    const float* e = envelopeBuffer.getReadPointer (0);
    
    const double period = 1.0 / frequency;
    const double periodInSamples = period * sampleRate;
    const double delta = 1.0 / periodInSamples;
    
    if (parser != nullptr)
    {
        for (int i = 0; i < numSamples; i++)
        {
            envelope = e[i];
            
            phase += delta;
            if (phase >= 1.0)
                phase -= 1.0;
            
            auto out = parser->evaluate();
            
            *l++ += out * gain;
            *r++ += out * gain;
        }
    }
}
