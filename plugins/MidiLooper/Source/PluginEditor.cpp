#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    const char* playSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M8 5 L19 12 L8 19 Z" fill="currentColor" stroke="currentColor" stroke-width="1" stroke-linejoin="round"/></svg>)";
    const char* stopSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2" ry="2" fill="currentColor"/></svg>)";
    const char* menuSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="4" y="6" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="11" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="16" width="16" height="2" rx="1" fill="currentColor"/></svg>)";
}

MidiLooperAudioProcessorEditor::MidiLooperAudioProcessorEditor (MidiLooperAudioProcessor& p)
    : gin::ProcessorEditor (p),
      proc (p),
      midiPlay ("play", playSVG, 4),
      midiStop ("stop", stopSVG, 4),
      midiMenu ("menu", menuSVG, 4)
{
    midiFile.onFileDropped = [this]
    {
        updatePlayStopButtons();
    };
    midiFile.onPlaybackStateChanged = [this] { updatePlayStopButtons(); };

    addAndMakeVisible (midiFile);

    midiPlay.setTooltip ("Play MIDI file");
    midiPlay.onClick = [this]
    {
        proc.midiPlayer.play();
        updatePlayStopButtons();
    };
    addAndMakeVisible (midiPlay);

    midiStop.setTooltip ("Stop MIDI playback");
    midiStop.onClick = [this]
    {
        proc.midiPlayer.stop();
        proc.midiPlayer.setPlayheadPosition (0.0);
        updatePlayStopButtons();
    };
    addAndMakeVisible (midiStop);

    midiMenu.setTooltip ("MIDI options");
    midiMenu.onClick = [this] { showMenu(); };
    addAndMakeVisible (midiMenu);

    updatePlayStopButtons();

    setGridSize (8, 1);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
}

MidiLooperAudioProcessorEditor::~MidiLooperAudioProcessorEditor()
{
}

void MidiLooperAudioProcessorEditor::updatePlayStopButtons()
{
    const bool isPlaying = proc.midiPlayer.isPlaying();
    midiPlay.setEnabled (! isPlaying && proc.midiPlayer.hasFileLoaded());
    midiStop.setEnabled (isPlaying || proc.midiPlayer.getPlayheadPosition() > 0.0);
}

void MidiLooperAudioProcessorEditor::showMenu()
{
    juce::PopupMenu menu;

    const bool isLooping = proc.midiPlayer.isLooping();
    menu.addItem ("Loop", true, isLooping, [this, isLooping]
    {
        proc.midiPlayer.setLooping (! isLooping);
    });

    menu.addSeparator();

    menu.addItem ("Load File...", [this]
    {
        auto fc = std::make_shared<juce::FileChooser> ("Load MIDI File",
            juce::File::getSpecialLocation (juce::File::userDocumentsDirectory),
            "*.mid;*.midi");

        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        fc->launchAsync (flags, [this, fc] (const juce::FileChooser&)
        {
            auto file = fc->getResult();
            if (file.existsAsFile())
            {
                proc.midiPlayer.load (file);
                updatePlayStopButtons();
                midiFile.repaint();
            }
        });
    });

    menu.addItem ("Clear File", proc.midiPlayer.hasFileLoaded(), false, [this]
    {
        proc.midiPlayer.clear();
        updatePlayStopButtons();
        midiFile.repaint();
    });

    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (midiMenu));
}

void MidiLooperAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto rc = juce::Rectangle<int> (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);

    auto buttonArea = rc.removeFromBottom (20);
    buttonArea.removeFromTop (4);

    midiPlay.setBounds (buttonArea.removeFromLeft (16));
    buttonArea.removeFromLeft (2);
    midiStop.setBounds (buttonArea.removeFromLeft (16));
    buttonArea.removeFromLeft (2);
    midiMenu.setBounds (buttonArea.removeFromLeft (16));

    midiFile.setBounds (rc);
}
