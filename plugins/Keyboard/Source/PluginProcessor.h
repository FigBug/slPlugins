#pragma once

#include <JuceHeader.h>

class KeyboardAudioProcessorEditor;

class ControllerState
{
public:
    void processBuffer (juce::MidiBuffer& buffer)
    {
        {
            juce::ScopedLock sl (lock);
            for (auto& m : messages)
                buffer.addEvent (m, 0);
            messages.clear();
        }

        juce::MidiBuffer::Iterator itr (buffer);
        juce::MidiMessage msg;
        int time;
        
        while (itr.getNextEvent (msg, time))
        {
            if (msg.isController())
                controllers[msg.getControllerNumber()] = msg.getControllerValue();
            else if (msg.isPitchWheel())
                pitchBend = msg.getPitchWheelValue();
        }
    }
    
    void addMessage (const juce::MidiMessage& m)
    {
        juce::ScopedLock sl (lock);
        messages.add (m);
    }
    
    int getControllerValue (int c)
    {
        return controllers[c];
    }
    
    int getPitchBend()
    {
        return pitchBend;
    }
    
private:
    juce::CriticalSection lock;
    juce::Array<juce::MidiMessage> messages;
    int controllers[128] = {0};
    int pitchBend = 0;
};

//==============================================================================
/**
*/
class KeyboardAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    KeyboardAudioProcessor();
    ~KeyboardAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    bool isMidiEffect() const override { return true; }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    juce::MidiKeyboardState keyboardState;
    ControllerState controllerState;

private:
    juce::Component::SafePointer<KeyboardAudioProcessorEditor> editor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardAudioProcessor)
};
