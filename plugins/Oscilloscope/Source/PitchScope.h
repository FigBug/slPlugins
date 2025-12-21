#pragma once

#include <JuceHeader.h>

class PluginProcessor;

//==============================================================================
/** TriggeredScope subclass that displays detected pitch info */
class PitchScope : public juce::Component,
                   private juce::Timer
{
public:
    PitchScope (gin::AudioFifo& fifo, PluginProcessor& proc);
    ~PitchScope() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    gin::TriggeredScope& getScope() { return scope; }

private:
    void timerCallback() override;

    static juce::String midiNoteToString (int noteNumber);
    static int frequencyToMidiNote (float frequency);

    gin::TriggeredScope scope;
    PluginProcessor& processor;
    float lastPitch = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchScope)
};
