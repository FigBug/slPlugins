#pragma once

#include <JuceHeader.h>

class KeyboardAudioProcessorEditor;

class ControllerState
{
public:
    void processBuffer (MidiBuffer& buffer)
    {
        {
            ScopedLock sl (lock);
            for (auto& m : messages)
                buffer.addEvent (m, 0);
            messages.clear();
        }
        
        MidiBuffer::Iterator itr (buffer);
        MidiMessage msg;
        int time;
        
        while (itr.getNextEvent (msg, time))
        {
            if (msg.isController())
                controllers[msg.getControllerNumber()] = msg.getControllerValue();
            else if (msg.isPitchWheel())
                pitchBend = msg.getPitchWheelValue();
        }
    }
    
    void addMessage (const MidiMessage& m)
    {
        ScopedLock sl (lock);
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
    CriticalSection lock;
    Array<MidiMessage> messages;
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

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    
    bool isMidiEffect() const override { return true; }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    MidiKeyboardState keyboardState;
    ControllerState controllerState;

private:
    Component::SafePointer<KeyboardAudioProcessorEditor> editor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardAudioProcessor)
};
