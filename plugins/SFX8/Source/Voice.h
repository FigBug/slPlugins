/*
  ==============================================================================

    Voice.h
    Created: 4 Jul 2019 8:25:21pm
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SFXAudioProcessor;
class Voice : public MPESynthesiserVoice
{
public:
    Voice (SFXAudioProcessor& s)
        : synth (s)
    {     
    }
    
    void noteStarted() override;
    
    void noteStopped (bool allowTailOff) override
    {
        if (! allowTailOff)
            clearCurrentNote();
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    void notePressureChanged() override     {}
    void notePitchbendChanged() override    {}
    void noteTimbreChanged() override       {}
    void noteKeyStateChanged() override     {}
    
private:
    SFXAudioProcessor& synth;
	SfxrSynth sfxr {44100.0f};
};
