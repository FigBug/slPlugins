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
    
    void setSampleRate (double sr)      { sampleRate = sr;  }
    
    void process (AudioSampleBuffer& buffer, int startSample, int numSamples);
    
private:
    double sampleRate = 44100.0;
};
