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
class Oscillator : public gin::AudioFunctionHost
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

    CriticalSection lock;
    std::unique_ptr<gin::EquationParser> parser;
    
    gin::ValueSmoother<double> controllers[128];
    
    double note = 0, frequency = 8.18, envelope = 1, gain = 1;
    double cutoff = 6000, res = 0;
};
