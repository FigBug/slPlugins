/*
  ==============================================================================

    Oscillator.h
    Created: 7 Jun 2018 11:03:19am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class Oscillator
{
public:
    //==============================================================================
    Oscillator();
    ~Oscillator();
    
    void setFormula (String formula);
    
    void start();
    
    void setController (int c, float v)     { controllers[c].setValue (v);                       }
    void setSampleRate (double sr);
    void setNote (double n)                 { note = n; frequency = gin::getMidiNoteInHertz (n); }
    void setGain (double g)                 { gain = g;                                          }
    void setCutoffRes (double c, double r)  { cutoff = c; res = r;                               }
    
    void process (AudioSampleBuffer& envelope, AudioSampleBuffer& buffer, int startSample, int numSamples);
    
private:
    //==============================================================================
    struct FuncState
    {
        FuncState (double sr) : sampleRate (sr) {}
        virtual ~FuncState() = default;
        virtual void setSampleRate (double sr) { sampleRate = sr; }
        double sampleRate = 44100.0;
    };
    
    //==============================================================================
    struct OscState : public FuncState
    {
        OscState (double sr) : FuncState (sr) {}
        
        double phase { Random::getSystemRandom().nextDouble() };
        
        inline void incPhase (double note)
        {
            if (note != lastNote)
            {
                lastNote = note;
                
                frequency = gin::getMidiNoteInHertz (note);
                const double period = 1.0 / frequency;
                const double periodInSamples = period * sampleRate;
                delta = 1.0 / periodInSamples;
                
                jassert (delta > 0);                
            }
            phase += delta;
            
            jassert (! std::isinf (phase));
        }
        
        double lastNote = -1, frequency = -1, delta = -1;
    };
    
    //==============================================================================
    struct HPState : public FuncState
    {
        HPState (double sr) : FuncState (sr) {}
        
        double process (double v, double freq, double res)
        {
            float q = 0.70710678118655f / (1.0f - (res / 100.0f) * 0.99f);
            auto c = IIRCoefficients::makeHighPass (sampleRate, freq, q);
            
            filter.setCoefficients (c);
            return filter.processSingleSampleRaw (v);
        }
        
        juce::IIRFilter filter;
    };

    //==============================================================================
    struct BPState : public FuncState
    {
        BPState (double sr) : FuncState (sr) {}
        
        double process (double v, double freq, double res)
        {
            float q = 0.70710678118655f / (1.0f - (res / 100.0f) * 0.99f);
            auto c = IIRCoefficients::makeBandPass (sampleRate, freq, q);
            
            filter.setCoefficients (c);
            return filter.processSingleSampleRaw (v);
        }
        
        juce::IIRFilter filter;
    };
    
    //==============================================================================
    struct LPState : public FuncState
    {
        LPState (double sr) : FuncState (sr) {}
        
        double process (double v, double freq, double res)
        {
            float q = 0.70710678118655f / (1.0f - (res / 100.0f) * 0.99f);
            auto c = IIRCoefficients::makeLowPass (sampleRate, freq, q);
            
            filter.setCoefficients (c);
            return filter.processSingleSampleRaw (v);
        }
        
        juce::IIRFilter filter;
    };
    
    //==============================================================================
    struct NotchState : public FuncState
    {
        NotchState (double sr) : FuncState (sr) {}
        
        double process (double v, double freq, double res)
        {
            float q = 0.70710678118655f / (1.0f - (res / 100.0f) * 0.99f);
            auto c = IIRCoefficients::makeNotchFilter (sampleRate, freq, q);
            
            filter.setCoefficients (c);
            return filter.processSingleSampleRaw (v);
        }
        
        juce::IIRFilter filter;
    };
    CriticalSection lock;
    std::unique_ptr<gin::EquationParser> parser;
    
    gin::ValueSmoother<double> controllers[128];
    std::map<int, std::unique_ptr<FuncState>> funcStates;
    
    template <class T>
    T* getFuncParams (int i, double sr)
    {
        auto itr = funcStates.find (i);
        if (itr != funcStates.end())
        {
            auto p = dynamic_cast<T*> (itr->second.get());
            jassert (p != nullptr);
            return p;
        }
        
        auto p = new T (sr);
        funcStates[i].reset (p);
        return p;
    }
    
    double note = 0, frequency = 8.18, envelope = 1, gain = 1;
    double cutoff = 6000, res = 0;
    double sampleRate = 44100.0;
};
