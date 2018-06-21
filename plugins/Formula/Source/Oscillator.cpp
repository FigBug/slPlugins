/*
  ==============================================================================

    Oscillator.cpp
    Created: 7 Jun 2018 11:03:19am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "Oscillator.h"
#include <random>

double gSampleRate = 44100.0;

//==============================================================================
inline bool oddEven (int x)
{
    return (x % 2 == 0) ? 1 : -1;
}

//==============================================================================
static double sine (double phase)
{
    return std::sin (phase * 2 * MathConstants<double>::pi);
}

static double triangle (double phase, double freq)
{
    double sum = 0;
    int k = 1;
    while (freq * k < gSampleRate / 2)
    {
        sum += std::pow (-1, (k - 1) / 2.0f) / (k * k) * std::sin (k * (phase * 2 * MathConstants<double>::pi));
        k += 2;
    }
    return float (8.0f / (float_Pi * float_Pi) * sum);
};

static double sawUp (double phase, double freq)
{
    double sum = 0;
    int k = 1;
    while (freq * k < gSampleRate / 2)
    {
        sum += oddEven (k) * std::sin (k * (phase * 2 * MathConstants<double>::pi)) / k;
        k++;
    }
    return float (-2.0f / float_Pi * sum);
};

static double sawDown (double phase, double freq)
{
    double sum = 0;
    int k = 1;
    while (freq * k < gSampleRate / 2)
    {
        sum += oddEven (k) * std::sin (k * (phase * 2 * MathConstants<double>::pi)) / k;
        k++;
    }
    return float (2.0f / float_Pi * sum);
};

static double pulse (double phase, double pw, double freq)
{
    if (pw == 0.5)
    {
        double sum = 0;
        int i = 1;
        while (freq * (2 * i - 1) < gSampleRate / 2)
        {
            sum += std::sin ((2 * i - 1) * (phase * 2 * MathConstants<double>::pi)) / ((2 * i - 1));
            i++;
        }
        
        return float (4.0f / float_Pi * sum);
    }
    else
    {
        return sawUp (phase + 0.5 * pw, freq) - sawUp (phase - 0.5 * pw, freq);
    }
};

static double noise()
{
    const float mean = 0.0f;
    const float stddev = 0.1f;
    
    static std::default_random_engine generator;
    static std::normal_distribution<float> dist (mean, stddev);
    
    return dist (generator);
};

//==============================================================================
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
    p->addFunction ("sawUp", sawUp);
    p->addFunction ("sawDown", sawDown);
    p->addFunction ("pulse", pulse);
    p->addFunction ("triangle", triangle);
    p->addFunction ("noise", noise);

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
    
    gSampleRate = sampleRate;
    
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
