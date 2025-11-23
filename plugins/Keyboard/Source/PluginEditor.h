#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ControllerComponent : public juce::Component,
                            private juce::Timer
{
public:
    ControllerComponent (ControllerState& cs, int controller_)
      : state (cs), controller (controller_)
    {
        startTimerHz (30);
    }
    
    void timerCallback() override
    {
        int newValue = controller >= 0 ? state.getControllerValue (controller) : state.getPitchBend();
        if (newValue != value)
        {
            value = newValue;
            repaint();
        }
    }
    
    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::white);
        g.drawRect (getLocalBounds());

        float p = controller >= 0 ? value / 127.0f : value / 16384.0f;

        int h = getHeight() - 2;
        int w = getWidth() - 2;

        g.setColour (juce::Colours::white.withAlpha (0.75f));
        g.drawRect (1, int (1 + h - h * p), w, 1);
    }
    
    ControllerState& state;
    int controller = 0;
    int value = 0;
};
//==============================================================================
/**
*/
class KeyboardAudioProcessorEditor  : public gin::ProcessorEditor
{
public:
    KeyboardAudioProcessorEditor (KeyboardAudioProcessor&);
    ~KeyboardAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    
private:
    KeyboardAudioProcessor& proc;
    juce::MidiKeyboardComponent keyboard;
    ControllerComponent modWheel, pitchWheel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardAudioProcessorEditor)
};
