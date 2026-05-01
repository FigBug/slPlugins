#pragma once

#include <JuceHeader.h>

#define PARAM_SHOW_NOTE_ON    "showNoteOn"
#define PARAM_SHOW_NOTE_OFF   "showNoteOff"
#define PARAM_SHOW_CC         "showCC"
#define PARAM_SHOW_PITCH_BEND "showPitchBend"
#define PARAM_SHOW_PRESSURE   "showPressure"
#define PARAM_SHOW_SYSTEM     "showSystem"
#define PARAM_SHOW_OTHER      "showOther"

//==============================================================================
struct NoteState
{
    void reset()
    {
        std::fill_n (*activeNotes, 16 * 128, 0);
    }

    bool addNote (int channel, int noteNumber)
    {
        jassert (channel >= 1 && channel <= 16);
        jassert (juce::isPositiveAndBelow (noteNumber, 128));
        ++activeNotes[channel - 1][noteNumber];

        return activeNotes[channel - 1][noteNumber] > 1;
    }

    void removeNote (int channel, int noteNumber)
    {
        jassert (channel >= 1 && channel <= 16);
        jassert (juce::isPositiveAndBelow (noteNumber, 128));
        --activeNotes[channel - 1][noteNumber];
        if (activeNotes[channel - 1][noteNumber] < 0)
            activeNotes[channel - 1][noteNumber] = 0;
    }

    void clearChannel (int channel)
    {
        jassert (channel >= 1 && channel <= 16);
        for (int n = 0; n < 128; n++)
            activeNotes[channel - 1][n] = 0;
    }

    bool isChannelOn (int channel) const
    {
        jassert (channel >= 1 && channel <= 16);
        for (int n = 0; n < 128; n++)
            if (activeNotes[channel - 1][n] > 0)
                return true;

        return false;
    }

    bool isEmpty() const
    {
        for (int c = 0; c < 16; ++c)
            for (int i = 0; i < 128; i++)
                if (activeNotes[c][i] > 0)
                    return false;

        return true;
    }

    juce::String getActiveNotesDisplay() const
    {
        juce::String s;
        bool comma = false;

        for (int c = 0; c < 16; ++c)
        {
            for (int i = 0; i < 128; i++)
            {
                if (activeNotes[c][i] > 0)
                {
                    if (comma)
                        s += ", ";

                    comma = true;
                    auto name = juce::MidiMessage::getMidiNoteName (i, true, true, 3);
                    s << name << " / " << juce::String (i) << " (" << (c + 1) << ") " << " x" << activeNotes[c][i];
                }
            }
        }

        return s;
    }

    int activeNotes[16][128] = {};
};

//==============================================================================
class MidiMonitorAudioProcessorEditor;

class MidiMonitorAudioProcessor : public gin::Processor,
                                  private juce::AsyncUpdater
{
public:
    MidiMonitorAudioProcessor();
    ~MidiMonitorAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    void clearLog();

    NoteState& getNoteState() { return noteState; }

    juce::Array<std::pair<juce::MidiMessage, int>> midiMessageList;

    juce::MidiKeyboardState keyboardState;

    gin::Parameter::Ptr showNoteOn    = nullptr;
    gin::Parameter::Ptr showNoteOff   = nullptr;
    gin::Parameter::Ptr showCC        = nullptr;
    gin::Parameter::Ptr showPitchBend = nullptr;
    gin::Parameter::Ptr showPressure  = nullptr;
    gin::Parameter::Ptr showSystem    = nullptr;
    gin::Parameter::Ptr showOther     = nullptr;

private:
    void handleAsyncUpdate() override;

    juce::Array<std::pair<juce::MidiMessage, int>> pendingMidiMessageList;
    juce::CriticalSection pendingLock;
    int count = 0;
    double currentSampleRate = 44100.0;
    double processedSeconds = 0.0;

    NoteState noteState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMonitorAudioProcessor)
};
