/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Pad.h"

using namespace juce::dsp;

class SFXAudioProcessorEditor;
//==============================================================================
class SFXAudioProcessor : public gin::GinProcessor,
                          private MPESynthesiser
{
public:
    //==============================================================================
    SFXAudioProcessor();
    ~SFXAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    void stateUpdated() override;
    void updateState() override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    Pad* getPadForNote (int note)
    {
        for (auto p : pads)
            if (p->note == note)
                return p;
        
        return nullptr;
    }

    Pad* getPad (int idx)               { return pads[idx]; }
    const OwnedArray<Pad>& getPads()    { return pads;      }
    
    String uniqueName (String prefix);

    //==============================================================================
    void midiNoteOn (int note, int velocity = 128);
    void midiNoteOff (int note, int velocity = 0);

    int isMidiNoteDown (int n)          { return midiOn[n] || midiCnt[n]; }

    int midiOn[128] = {0}, midiCnt[128] = {0};

private:
    void trackMidi (MidiBuffer& midi, int numSamples);
    //==============================================================================
    
    CriticalSection lock;    
    Component::SafePointer<SFXAudioProcessorEditor> editor;
    
    OwnedArray<Pad> pads;

    MidiBuffer userMidi;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SFXAudioProcessor)
};
