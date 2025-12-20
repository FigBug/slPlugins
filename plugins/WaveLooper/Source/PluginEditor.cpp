#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    const char* playSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M8 5 L19 12 L8 19 Z" fill="currentColor" stroke="currentColor" stroke-width="1" stroke-linejoin="round"/></svg>)";
    const char* stopSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2" ry="2" fill="currentColor"/></svg>)";
    const char* menuSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="4" y="6" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="11" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="16" width="16" height="2" rx="1" fill="currentColor"/></svg>)";
}

WaveLooperAudioProcessorEditor::WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor& p)
    : gin::ProcessorEditor (p),
      proc (p),
      samplePlay ("play", playSVG, 4),
      sampleStop ("stop", stopSVG, 4),
      sampleMenu ("menu", menuSVG, 4)
{
    sample.setBackgroundColour (findColour (gin::GinLookAndFeel::displayColourId));
    sample.setLineColour (findColour (gin::GinLookAndFeel::accentColourId));

    if (proc.samplePlayer.hasFileLoaded())
        sample.setBuffer (proc.samplePlayer.getBuffer());

    sample.onFileDropped = [this]
    {
        updatePlayStopButtons();
    };
    sample.onPlaybackStateChanged = [this] { updatePlayStopButtons(); };

    addAndMakeVisible (sample);

    samplePlay.setTooltip ("Play audio sample");
    samplePlay.onClick = [this]
    {
        proc.samplePlayer.play();
        updatePlayStopButtons();
    };
    addAndMakeVisible (samplePlay);

    sampleStop.setTooltip ("Stop sample playback");
    sampleStop.onClick = [this]
    {
        proc.samplePlayer.stop();
        proc.samplePlayer.setPosition (0.0);
        updatePlayStopButtons();
    };
    addAndMakeVisible (sampleStop);

    sampleMenu.setTooltip ("Sample options");
    sampleMenu.onClick = [this] { showMenu(); };
    addAndMakeVisible (sampleMenu);

    updatePlayStopButtons();

    setGridSize (8, 1);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
}

WaveLooperAudioProcessorEditor::~WaveLooperAudioProcessorEditor()
{
}

void WaveLooperAudioProcessorEditor::updatePlayStopButtons()
{
    const bool isPlaying = proc.samplePlayer.isPlaying();
    samplePlay.setEnabled (! isPlaying && proc.samplePlayer.hasFileLoaded());
    sampleStop.setEnabled (isPlaying || proc.samplePlayer.getPosition() > 0.0);
}

void WaveLooperAudioProcessorEditor::showMenu()
{
    juce::PopupMenu menu;

    const bool isLooping = proc.samplePlayer.isLooping();
    menu.addItem ("Loop", true, isLooping, [this, isLooping]
    {
        proc.samplePlayer.setLooping (! isLooping);
    });

    menu.addSeparator();

    menu.addItem ("Load File...", [this]
    {
        auto fc = std::make_shared<juce::FileChooser> ("Load Audio File",
            juce::File::getSpecialLocation (juce::File::userDocumentsDirectory),
            "*.wav;*.aiff;*.aif;*.mp3;*.ogg;*.flac");

        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        fc->launchAsync (flags, [this, fc] (const juce::FileChooser&)
        {
            auto file = fc->getResult();
            if (file.existsAsFile())
            {
                proc.samplePlayer.load (file);
                sample.setBuffer (proc.samplePlayer.getBuffer());
                updatePlayStopButtons();
                sample.repaint();
            }
        });
    });

    menu.addItem ("Clear File", proc.samplePlayer.hasFileLoaded(), false, [this]
    {
        proc.samplePlayer.clear();
        sample.clear();
        updatePlayStopButtons();
        sample.repaint();
    });

    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (sampleMenu));
}

void WaveLooperAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto rc = juce::Rectangle<int> (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);

    auto buttonArea = rc.removeFromBottom (20);
    buttonArea.removeFromTop (4);

    samplePlay.setBounds (buttonArea.removeFromLeft (16));
    buttonArea.removeFromLeft (2);
    sampleStop.setBounds (buttonArea.removeFromLeft (16));
    buttonArea.removeFromLeft (2);
    sampleMenu.setBounds (buttonArea.removeFromLeft (16));

    sample.setBounds (rc);
}
