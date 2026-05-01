#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    const char* resetSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M12 4 V1 L7.5 5.5 L12 10 V6 a6 6 0 1 1 -6 6 H4 a8 8 0 1 0 8 -8 Z" fill="currentColor"/></svg>)";
    const char* clearSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M9 3 H15 V5 H20 V7 H4 V5 H9 Z M6 8 H18 L17 20 a2 2 0 0 1 -2 2 H9 a2 2 0 0 1 -2 -2 Z M10 11 V18 H12 V11 Z M14 11 V18 H16 V11 Z" fill="currentColor"/></svg>)";
    const char* copySVG  = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M15 1 H4 a2 2 0 0 0 -2 2 V16 H4 V3 H15 Z M19 5 H8 a2 2 0 0 0 -2 2 V21 a2 2 0 0 0 2 2 H19 a2 2 0 0 0 2 -2 V7 a2 2 0 0 0 -2 -2 Z M19 21 H8 V7 H19 Z" fill="currentColor"/></svg>)";
    const char* saveSVG  = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M17 3 H5 a2 2 0 0 0 -2 2 V19 a2 2 0 0 0 2 2 H19 a2 2 0 0 0 2 -2 V7 Z M12 19 a3 3 0 1 1 0 -6 a3 3 0 1 1 0 6 Z M15 9 H5 V5 H15 Z" fill="currentColor"/></svg>)";

    juce::String formatTime (double seconds)
    {
        seconds = juce::jmax (0.0, seconds);
        return juce::String::formatted ("%02d:%02d:%02d.%03d",
            ((int) (seconds / 3600.0)) % 24,
            ((int) (seconds / 60.0)) % 60,
            ((int) seconds) % 60,
            ((int) (std::fmod (seconds, 1.0) * 1000)));
    }

    juce::String describeNote (const juce::MidiMessage& m)
    {
        return juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3)
               + " (" + juce::String (m.getNoteNumber()) + ")";
    }

    juce::String describeController (const juce::MidiMessage& m)
    {
        const int cc = m.getControllerNumber();
        const int v  = m.getControllerValue();

        switch (cc)
        {
            case 120: return "All Sound Off";
            case 121: return "Reset All Controllers";
            case 122: return juce::String ("Local Control ") + (v >= 64 ? "On" : "Off");
            case 123: return "All Notes Off";
            case 124: return "Omni Mode Off";
            case 125: return "Omni Mode On";
            case 126: return "Mono Mode On  channels " + juce::String (v);
            case 127: return "Poly Mode On";
            default:  break;
        }

        juce::String name (juce::MidiMessage::getControllerName (cc));
        if (name.isEmpty())
            name = "CC " + juce::String (cc);

        return "Controller  " + name + " (" + juce::String (cc) + ") = " + juce::String (v);
    }

    juce::String describeMetaEvent (const juce::MidiMessage& m)
    {
        if (m.isTempoMetaEvent())
            return "Tempo  " + juce::String (60.0 / m.getTempoSecondsPerQuarterNote(), 2) + " bpm";

        if (m.isTimeSignatureMetaEvent())
        {
            int num = 0, den = 0;
            m.getTimeSignatureInfo (num, den);
            return "Time Signature  " + juce::String (num) + "/" + juce::String (den);
        }

        if (m.isKeySignatureMetaEvent())
            return juce::String ("Key Signature  ") + (m.isKeySignatureMajorKey() ? "major " : "minor ")
                   + juce::String (m.getKeySignatureNumberOfSharpsOrFlats());

        if (m.isTrackNameEvent())
            return "Track Name  " + m.getTextFromTextMetaEvent();

        if (m.isTextMetaEvent())
            return "Text  " + m.getTextFromTextMetaEvent();

        if (m.isMidiChannelMetaEvent())
            return "Channel Prefix  " + juce::String (m.getMidiChannelMetaEventChannel());

        if (m.isEndOfTrackMetaEvent())
            return "End of Track";

        return "Meta Event  type 0x" + juce::String::toHexString (m.getMetaEventType()).paddedLeft ('0', 2).toUpperCase();
    }

    juce::StringArray getMidiMessageDescription (const juce::MidiMessage& m)
    {
        juce::StringArray cols;
        const int ch = m.getChannel();
        cols.add (ch > 0 ? ("Ch " + juce::String (ch)) : juce::String());

        if      (m.isNoteOn())              cols.add ("Note On   " + describeNote (m) + "  vel " + juce::String (m.getVelocity()));
        else if (m.isNoteOff())             cols.add ("Note Off  " + describeNote (m) + "  vel " + juce::String (m.getVelocity()));
        else if (m.isProgramChange())       cols.add ("Program Change  " + juce::String (m.getProgramChangeNumber()));
        else if (m.isPitchWheel())          cols.add ("Pitch Wheel  " + juce::String (m.getPitchWheelValue()));
        else if (m.isAftertouch())          cols.add ("Aftertouch  " + describeNote (m) + "  " + juce::String (m.getAfterTouchValue()));
        else if (m.isChannelPressure())     cols.add ("Channel Pressure  " + juce::String (m.getChannelPressureValue()));
        else if (m.isController())          cols.add (describeController (m));
        else if (m.isMetaEvent())           cols.add (describeMetaEvent (m));
        else if (m.isSysEx())               cols.add ("SysEx (" + juce::String (m.getSysExDataSize()) + " bytes)");
        else if (m.isMidiClock())           cols.add ("MIDI Clock");
        else if (m.isMidiStart())           cols.add ("MIDI Start");
        else if (m.isMidiStop())            cols.add ("MIDI Stop");
        else if (m.isMidiContinue())        cols.add ("MIDI Continue");
        else if (m.isSongPositionPointer()) cols.add ("Song Position  " + juce::String (m.getSongPositionPointerMidiBeat()));
        else if (m.isActiveSense())         cols.add ("Active Sense");
        else                                cols.add (m.getDescription());

        return cols;
    }

    juce::String getRawDescription (const juce::MidiMessage& m)
    {
        juce::String s;
        const auto* data = m.getRawData();
        const int size = juce::jmin (m.getRawDataSize(), 16);
        for (int i = 0; i < size; ++i)
        {
            if (i > 0)
                s += " ";
            s += juce::String::toHexString (data[i]).paddedLeft ('0', 2).toUpperCase();
        }
        if (m.getRawDataSize() > size)
            s += " ...";
        return s;
    }
}

//==============================================================================
class MidiMonitorAudioProcessorEditor::MidiLogListBoxModel : public juce::ListBoxModel
{
public:
    MidiLogListBoxModel (MidiMonitorAudioProcessorEditor& o)
        : owner (o)
    {
    }

    int getNumRows() override { return owner.filteredIndices.size(); }

    void paintListBoxItem (int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
        {
            auto c = owner.findColour (gin::GinLookAndFeel::accentColourId);
            g.fillAll (c.withAlpha (0.4f));
        }
        else if (row % 2 == 1)
        {
            g.fillAll (juce::Colours::black.withAlpha (0.2f));
        }

        if (! juce::isPositiveAndBelow (row, owner.filteredIndices.size()))
            return;

        const int srcIdx = owner.filteredIndices.getUnchecked (row);
        if (! juce::isPositiveAndBelow (srcIdx, owner.proc.midiMessageList.size()))
            return;

        g.setColour (owner.findColour (gin::GinLookAndFeel::whiteColourId));
        g.setFont (juce::Font (juce::Font::getDefaultMonospacedFontName(), (float) height - 3.0f, juce::Font::plain));

        const auto& item = owner.proc.midiMessageList.getReference (srcIdx);
        auto rc = juce::Rectangle<int> (width, height).reduced (4, 0);

        g.drawText (juce::String (item.second), rc.removeFromLeft (40), juce::Justification::centredRight, true);
        rc.removeFromLeft (8);

        g.drawText (formatTime (item.first.getTimeStamp()), rc.removeFromLeft (90), juce::Justification::centredLeft, true);
        rc.removeFromLeft (4);

        auto rawArea = rc.removeFromRight (160);
        g.drawText (getRawDescription (item.first), rawArea, juce::Justification::centredRight, true);
        rc.removeFromRight (8);

        for (const auto& t : getMidiMessageDescription (item.first))
        {
            const int colWidth = (t.length() < 12 ? 60 : 240);
            g.drawText (t, rc.removeFromLeft (colWidth), juce::Justification::centredLeft, true);
            rc.removeFromLeft (4);
            if (rc.isEmpty())
                break;
        }
    }

private:
    MidiMonitorAudioProcessorEditor& owner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLogListBoxModel)
};

//==============================================================================
MidiMonitorAudioProcessorEditor::MidiMonitorAudioProcessorEditor (MidiMonitorAudioProcessor& p)
    : gin::ProcessorEditor (p),
      proc (p),
      resetButton ("reset", resetSVG, 4),
      clearButton ("clear", clearSVG, 4),
      copyButton  ("copy",  copySVG,  4),
      saveButton  ("save",  saveSVG,  4)
{
    midiLogListBoxModel = std::make_unique<MidiLogListBoxModel> (*this);
    midiLogListBox.setRowHeight (16);
    midiLogListBox.setColour (juce::ListBox::backgroundColourId, findColour (gin::GinLookAndFeel::displayColourId));
    midiLogListBox.setModel (midiLogListBoxModel.get());
    addAndMakeVisible (midiLogListBox);

    activeNotesLabel.setColour (juce::Label::textColourId, findColour (gin::GinLookAndFeel::accentColourId));
    activeNotesLabel.setMinimumHorizontalScale (1.0f);
    addAndMakeVisible (activeNotesLabel);

    resetButton.setTooltip ("Clear active notes");
    resetButton.onClick = [this] { proc.getNoteState().reset(); };
    addAndMakeVisible (resetButton);

    clearButton.setTooltip ("Clear log");
    clearButton.onClick = [this] { proc.clearLog(); };
    addAndMakeVisible (clearButton);

    copyButton.setTooltip ("Copy log to clipboard");
    copyButton.onClick = [this] { copyLogToClipboard(); };
    addAndMakeVisible (copyButton);

    saveButton.setTooltip ("Save log to file...");
    saveButton.onClick = [this] { saveLogToFile(); };
    addAndMakeVisible (saveButton);

    auto setupFilterButton = [this] (juce::ToggleButton& b, gin::Parameter::Ptr p, const juce::String& tooltip)
    {
        b.setTooltip (tooltip);
        b.onClick = [&b, p]
        {
            p->setUserValueAsUserAction (b.getToggleState() ? 1.0f : 0.0f);
        };
        addAndMakeVisible (b);
    };

    setupFilterButton (filterNoteOnButton,    proc.showNoteOn,    "Show Note On messages");
    setupFilterButton (filterNoteOffButton,   proc.showNoteOff,   "Show Note Off messages");
    setupFilterButton (filterCCButton,        proc.showCC,        "Show Controller messages");
    setupFilterButton (filterPitchBendButton, proc.showPitchBend, "Show Pitch Bend messages");
    setupFilterButton (filterPressureButton,  proc.showPressure,  "Show Aftertouch / Channel Pressure messages");
    setupFilterButton (filterSystemButton,    proc.showSystem,    "Show System messages (Clock, Start/Stop, Song Position, Active Sense)");
    setupFilterButton (filterOtherButton,     proc.showOther,     "Show all other MIDI messages (Program Change, SysEx, Meta)");

    syncFilterButtonsFromParams();
    rebuildFilteredIndices();

    keyboard.setLowestVisibleKey (36);
    addAndMakeVisible (keyboard);

    titleBar.setShowPresets (false);

    proc.showNoteOn   ->addListener (this);
    proc.showNoteOff  ->addListener (this);
    proc.showCC       ->addListener (this);
    proc.showPitchBend->addListener (this);
    proc.showPressure ->addListener (this);
    proc.showSystem   ->addListener (this);
    proc.showOther    ->addListener (this);

    proc.addChangeListener (this);
    startTimerHz (25);

    setGridSize (11, 6, 80, 92);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
}

MidiMonitorAudioProcessorEditor::~MidiMonitorAudioProcessorEditor()
{
    stopTimer();
    proc.removeChangeListener (this);
    proc.showNoteOn   ->removeListener (this);
    proc.showNoteOff  ->removeListener (this);
    proc.showCC       ->removeListener (this);
    proc.showPitchBend->removeListener (this);
    proc.showPressure ->removeListener (this);
    proc.showSystem   ->removeListener (this);
    proc.showOther    ->removeListener (this);
}

void MidiMonitorAudioProcessorEditor::changeListenerCallback (juce::ChangeBroadcaster*)
{
    rebuildFilteredIndices();
    midiLogListBox.updateContent();
    midiLogListBox.repaint();
    midiLogListBox.setVerticalPosition (1.0);
}

void MidiMonitorAudioProcessorEditor::valueUpdated (gin::Parameter*)
{
    syncFilterButtonsFromParams();
    rebuildFilteredIndices();
    midiLogListBox.updateContent();
    midiLogListBox.repaint();
}

void MidiMonitorAudioProcessorEditor::syncFilterButtonsFromParams()
{
    filterNoteOnButton   .setToggleState (proc.showNoteOn   ->getUserValueBool(), juce::dontSendNotification);
    filterNoteOffButton  .setToggleState (proc.showNoteOff  ->getUserValueBool(), juce::dontSendNotification);
    filterCCButton       .setToggleState (proc.showCC       ->getUserValueBool(), juce::dontSendNotification);
    filterPitchBendButton.setToggleState (proc.showPitchBend->getUserValueBool(), juce::dontSendNotification);
    filterPressureButton .setToggleState (proc.showPressure ->getUserValueBool(), juce::dontSendNotification);
    filterSystemButton   .setToggleState (proc.showSystem   ->getUserValueBool(), juce::dontSendNotification);
    filterOtherButton    .setToggleState (proc.showOther    ->getUserValueBool(), juce::dontSendNotification);
}

bool MidiMonitorAudioProcessorEditor::messagePassesFilter (const juce::MidiMessage& m) const
{
    if (m.isNoteOn())              return proc.showNoteOn   ->getUserValueBool();
    if (m.isNoteOff())             return proc.showNoteOff  ->getUserValueBool();
    if (m.isController())          return proc.showCC       ->getUserValueBool();
    if (m.isPitchWheel())          return proc.showPitchBend->getUserValueBool();
    if (m.isAftertouch()
        || m.isChannelPressure())  return proc.showPressure ->getUserValueBool();
    if (m.isMidiClock()
        || m.isMidiStart()
        || m.isMidiStop()
        || m.isMidiContinue()
        || m.isSongPositionPointer()
        || m.isActiveSense())      return proc.showSystem   ->getUserValueBool();
    return proc.showOther->getUserValueBool();
}

void MidiMonitorAudioProcessorEditor::rebuildFilteredIndices()
{
    filteredIndices.clearQuick();
    filteredIndices.ensureStorageAllocated (proc.midiMessageList.size());

    for (int i = 0; i < proc.midiMessageList.size(); ++i)
        if (messagePassesFilter (proc.midiMessageList.getReference (i).first))
            filteredIndices.add (i);
}

void MidiMonitorAudioProcessorEditor::timerCallback()
{
    activeNotesLabel.setText ("Active Notes: " + proc.getNoteState().getActiveNotesDisplay(),
                              juce::dontSendNotification);
}

juce::String MidiMonitorAudioProcessorEditor::buildFilteredLogText() const
{
    juce::String text;
    for (int srcIdx : filteredIndices)
    {
        if (! juce::isPositiveAndBelow (srcIdx, proc.midiMessageList.size()))
            continue;

        const auto& itm = proc.midiMessageList.getReference (srcIdx);
        text += formatTime (itm.first.getTimeStamp());
        text += "\t";

        for (const auto& t : getMidiMessageDescription (itm.first))
            text += t + "\t";

        text += getRawDescription (itm.first) + "\n";
    }
    return text;
}

void MidiMonitorAudioProcessorEditor::copyLogToClipboard()
{
    juce::SystemClipboard::copyTextToClipboard (buildFilteredLogText());
}

void MidiMonitorAudioProcessorEditor::saveLogToFile()
{
    fileChooser = std::make_shared<juce::FileChooser> ("Save MIDI Log",
        juce::File::getSpecialLocation (juce::File::userDocumentsDirectory).getChildFile ("midi_log.txt"),
        "*.txt");

    auto flags = juce::FileBrowserComponent::saveMode
               | juce::FileBrowserComponent::canSelectFiles
               | juce::FileBrowserComponent::warnAboutOverwriting;

    fileChooser->launchAsync (flags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file == juce::File())
            return;

        file.replaceWithText (buildFilteredLogText());
    });
}

void MidiMonitorAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto rc = juce::Rectangle<int> (inset, headerHeight + inset,
                                    getWidth() - 2 * inset,
                                    getHeight() - headerHeight - 2 * inset);

    auto topR = rc.removeFromTop (20);
    saveButton.setBounds  (topR.removeFromRight (20));
    topR.removeFromRight (2);
    copyButton.setBounds  (topR.removeFromRight (20));
    topR.removeFromRight (2);
    clearButton.setBounds (topR.removeFromRight (20));
    topR.removeFromRight (2);
    resetButton.setBounds (topR.removeFromRight (20));
    topR.removeFromRight (4);
    activeNotesLabel.setBounds (topR);

    rc.removeFromTop (4);

    auto filterRow = rc.removeFromTop (24);
    {
        juce::Font font (juce::FontOptions (15.0f));
        const int tickAndPadding = 30; // ~4 (left) + 16.5 (tick) + 10 (gap)
        const int gap = 12;

        auto place = [&] (juce::ToggleButton& b)
        {
            const int w = tickAndPadding + juce::GlyphArrangement::getStringWidthInt (font, b.getButtonText());
            b.setBounds (filterRow.removeFromLeft (w));
            filterRow.removeFromLeft (gap);
        };

        place (filterNoteOnButton);
        place (filterNoteOffButton);
        place (filterCCButton);
        place (filterPitchBendButton);
        place (filterPressureButton);
        place (filterSystemButton);
        place (filterOtherButton);
    }

    rc.removeFromTop (4);
    keyboard.setBounds (rc.removeFromBottom (64));
    rc.removeFromBottom (4);
    midiLogListBox.setBounds (rc);
}
