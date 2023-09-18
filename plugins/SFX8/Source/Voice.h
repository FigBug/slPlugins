/*
  ==============================================================================

    Voice.h
    Created: 4 Jul 2019 8:25:21pm
    Author:  Roland Rabien

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../../modules/bfxr/PinkNumber.h"
#include "../../../modules/bfxr/SfxrParams.h"
#include "../../../modules/bfxr/SfxrSynth.h"

class SFXAudioProcessor;
class Voice : public juce::MPESynthesiserVoice
{
public:
    Voice (SFXAudioProcessor& s)
        : synth (s)
    {
    }

    void setCurrentSampleRate (double newRate) override
    {
        MPESynthesiserVoice::setCurrentSampleRate (newRate);

        if (newRate != 44100.0 && newRate > 0)
            resamplingFifo = std::make_unique<gin::ResamplingFifo> (64, 1);
        else
            resamplingFifo = nullptr;
    }

    void noteStarted() override;

    void noteStopped (bool allowTailOff) override
    {
        if (! allowTailOff)
            clearCurrentNote();
    }

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void notePressureChanged() override     {}
    void notePitchbendChanged() override    {}
    void noteTimbreChanged() override       {}
    void noteKeyStateChanged() override     {}

private:
    SFXAudioProcessor& synth;
    SfxrSynth sfxr {44100.0f};

    std::unique_ptr<gin::ResamplingFifo> resamplingFifo;
};
