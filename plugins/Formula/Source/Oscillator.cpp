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
        pw = jlimit (0.05, 0.95, pw);
        return sawUp (phase + 0.5 * pw, freq) - sawUp (phase - 0.5 * pw, freq);
    }
};

static double squareWave (double phase, double freq)
{
    double sum = 0;
    int i = 1;
    while (freq * (2 * i - 1) < gSampleRate / 2)
    {
        sum += std::sin ((2 * i - 1) * (phase * 2 * MathConstants<double>::pi)) / ((2 * i - 1));
        i++;
    }
    
    return float (4.0f / float_Pi * sum);
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

void Oscillator::setSampleRate (double sr)
{
    jassert (sr > 8000);    
    sampleRate = sr;
    
    for (auto& c : controllers)
        c.reset (sr, 0.05);
    
    for (auto& itr : funcStates)
        itr.second->setSampleRate (sr);
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
        p->addVariable (String::formatted ( "cc%d", i), controllers[i].getValuePtr());
    
    p->addFunction ("sine", [this] (int id, double note)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return sine (p->phase);
                    });
    p->addFunction ("saw", [this] (int id, double note)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return sawUp (p->phase, p->frequency);
                    });
    p->addFunction ("sawdown", [this] (int id, double note)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return sawDown (p->phase, p->frequency);
                    });
    p->addFunction ("pulse", [this] (int id, double note, double pw)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return pulse (p->phase, p->frequency, pw);
                    });
    p->addFunction ("square", [this] (int id, double note)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return squareWave (p->phase, p->frequency);
                    });
    p->addFunction ("triangle", [this] (int id, double note)
                    {
                        auto p = getFuncParams<OscState> (id, sampleRate);
                        p->incPhase (note);
                        return triangle (p->phase, p->frequency);
                    });
    p->addFunction ("noise", [this] (int, double note)
                    {
                        return noise();
                    });
    p->addFunction ("hp", [this] (int id, double v, double freq, double res)
                    {
                        auto p = getFuncParams<HPState> (id, sampleRate);
                        float q = 0.70710678118655f / (1.0f - (res) * 0.99f);
                        return p->process (v, freq, q);
                    });
    p->addFunction ("lp", [this] (int id, double v, double freq, double res)
                    {
                        auto p = getFuncParams<LPState> (id, sampleRate);
                        float q = 0.70710678118655f / (1.0f - (res) * 0.99f);
                        return p->process (v, freq, q);
                    });
    p->addFunction ("notch", [this] (int id, double v, double freq, double res)
                    {
                        auto p = getFuncParams<NotchState> (id, sampleRate);
                        float q = 0.70710678118655f / (1.0f - (res) * 0.99f);
                        return p->process (v, freq, q);
                    });
    p->addFunction ("bp", [this] (int id, double v, double freq, double res)
                    {
                        auto p = getFuncParams<BPState> (id, sampleRate);
                        float q = 0.70710678118655f / (1.0f - (res) * 0.99f);
                        return p->process (v, freq, q);
                    });

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
}

void Oscillator::process (AudioSampleBuffer& envelopeBuffer, AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    ScopedLock sl (lock);
    
    gSampleRate = sampleRate;
    
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
