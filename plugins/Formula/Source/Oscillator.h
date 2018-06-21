/*
  ==============================================================================

    Oscillator.h
    Created: 7 Jun 2018 11:03:19am
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator
{
public:
    Oscillator();
    ~Oscillator();
    
    void setFormula (String formula);
    
    void start();
    
    void setSampleRate (double sr)      { sampleRate = sr;  }
    void setFrequency (double f)        { frequency = f;    }
    void setGain (double g)             { gain = g;         }
    
    void process (AudioSampleBuffer& envelope, AudioSampleBuffer& buffer, int startSample, int numSamples);
    
private:
    CriticalSection lock;
    ScopedPointer<gin::EquationParser> parser;
    
    double frequency = 0, phase = 0, envelope = 1, gain = 1;
    
    double sampleRate = 44100.0;
};
