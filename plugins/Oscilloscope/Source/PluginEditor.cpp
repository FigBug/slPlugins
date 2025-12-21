#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    const char* playSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M8 5v14l11-7z" fill="currentColor"/></svg>)";
    const char* pauseSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="6" y="4" width="4" height="16" fill="currentColor"/><rect x="14" y="4" width="4" height="16" fill="currentColor"/></svg>)";
    const char* fullscreenSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M7 14H5v5h5v-2H7v-3zm-2-4h2V7h3V5H5v5zm12 7h-3v2h5v-5h-2v3zM14 5v2h3v3h2V5h-5z" fill="currentColor"/></svg>)";
    const char* exitFullscreenSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path d="M5 16h3v3h2v-5H5v2zm3-8H5v2h5V5H8v3zm6 11h2v-3h3v-2h-5v5zm2-11V5h-2v5h5V8h-3z" fill="currentColor"/></svg>)";
    const char* recordSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><circle cx="12" cy="12" r="8" fill="currentColor"/></svg>)";
    const char* triggerSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path fill-rule="evenodd" d="M12 4a8 8 0 1 0 0 16 8 8 0 0 0 0-16zM8 8h8v2h-3v7h-2v-7H8V8z" fill="currentColor"/></svg>)";
    const char* retroSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path fill-rule="evenodd" d="M12 4a8 8 0 1 0 0 16 8 8 0 0 0 0-16zM9 7h5a3 3 0 0 1 .5 5.96L17 17h-2.5l-2.3-4H11v4H9V7zm2 2v2h3a1 1 0 1 0 0-2h-3z" fill="currentColor"/></svg>)";
    const char* stopSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2" ry="2" fill="currentColor"/></svg>)";
    const char* menuSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="4" y="6" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="11" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="16" width="16" height="2" rx="1" fill="currentColor"/></svg>)";
}

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), scopeProc (p),
      playPauseButton ("playPause", playSVG, 4),
      fullscreenButton ("fullscreen", fullscreenSVG, 4),
      recordNormal ("record", recordSVG, 4),
      recordTriggered ("trigger", triggerSVG, 4),
      recordRetrospective ("retro", retroSVG, 4),
      recordStop ("stop", stopSVG, 4),
      recordMenu ("menu", menuSVG, 4)
{
    addAndMakeVisible (&scope);

    auto& innerScope = scope.getScope();

    innerScope.setColour (gin::TriggeredScope::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    innerScope.setDrawCursorInfo (true);
    innerScope.setPlayheadSource ([&p]() -> std::tuple<double, double, bool> {
        return { p.lastPpqPosition.load(), p.lastBpm.load(), p.lastIsPlaying.load() };
    });

    for (auto pp : p.getPluginParameters())
    {
        gin::ParamComponent* pc;

        if (pp == p.triggerMode || pp == p.triggerChannel || pp == p.triggerRun)
            pc = new gin::Select (pp);
        else if (pp->isOnOff())
            pc = new gin::Switch (pp);
        else
            pc = new gin::Knob (pp);

        addAndMakeVisible (pc);
        controls.add (pc);
    }

    // Play/Pause button
    playPauseButton.setTooltip ("Play/Pause scope (freeze display)");
    playPauseButton.onClick = [this]
    {
        auto& triggeredScope = scope.getScope();

        if (scopeProc.triggerRun->isOn())
        {
            // In single trigger mode - reset trigger to capture again
            triggeredScope.resetTrigger();
        }
        else
        {
            // Normal mode - toggle pause
            triggeredScope.setPaused (! triggeredScope.isPaused());
        }
        updatePlayPauseButton();
    };
    addAndMakeVisible (playPauseButton);

    // Fullscreen button
    fullscreenButton.setTooltip ("Toggle fullscreen");
    fullscreenButton.onClick = [this]
    {
        setFullscreen (! isFullscreen);
    };
    addAndMakeVisible (fullscreenButton);

    // Recording UI
    recordNormal.setTooltip ("Start/stop recording");
    recordNormal.onClick = [this]
    {
        if (scopeProc.audioRecorder.isRecording())
            scopeProc.audioRecorder.stopRecording();
        else
            scopeProc.audioRecorder.startRecording();
        updateRecordButtons();
    };
    addAndMakeVisible (recordNormal);

    recordTriggered.setTooltip ("Triggered recording (starts when audio detected)");
    recordTriggered.onClick = [this]
    {
        if (scopeProc.audioRecorder.isRecording() || scopeProc.audioRecorder.isWaitingForTrigger())
            scopeProc.audioRecorder.stopRecording();
        else
            scopeProc.audioRecorder.startTriggeredRecording();
        updateRecordButtons();
    };
    addAndMakeVisible (recordTriggered);

    recordRetrospective.setTooltip ("Save last 30 seconds to file");
    recordRetrospective.onClick = [this]
    {
        scopeProc.audioRecorder.saveRetrospective();
    };
    addAndMakeVisible (recordRetrospective);

    recordStop.setTooltip ("Stop recording");
    recordStop.onClick = [this]
    {
        scopeProc.audioRecorder.stopRecording();
        updateRecordButtons();
    };
    addAndMakeVisible (recordStop);

    recordMenu.setTooltip ("Recording options");
    recordMenu.onClick = [this] { showRecordMenu(); };
    addAndMakeVisible (recordMenu);

    // Colour swatches
    auto defaultColour1 = juce::Colours::white.overlaidWith (juce::Colours::blue.withAlpha (0.3f));
    auto defaultColour2 = juce::Colours::white.overlaidWith (juce::Colours::yellow.withAlpha (0.3f));

    colourSwatch1.setTooltip ("Left channel colour (shift-click to reset)");
    colourSwatch1.setDefaultColour (defaultColour1);
    colourSwatch1.onColourChanged = [this] (juce::Colour c)
    {
        scopeProc.setTraceColour (0, c);
    };
    addAndMakeVisible (colourSwatch1);

    colourSwatch2.setTooltip ("Right channel colour (shift-click to reset)");
    colourSwatch2.setDefaultColour (defaultColour2);
    colourSwatch2.onColourChanged = [this] (juce::Colour c)
    {
        scopeProc.setTraceColour (1, c);
    };
    addAndMakeVisible (colourSwatch2);

    // Update colours when processor state changes
    scopeProc.onTraceColourChanged = [this] { updateTraceColours(); };

    scopeProc.audioRecorder.onRecordingStarted = [this] { updateRecordButtons(); updateRecordButtonColours(); };
    scopeProc.audioRecorder.onRecordingStopped = [this] { updateRecordButtons(); updateRecordButtonColours(); };

    startTimerHz (6);
    updateRecordButtons();

    setGridSize (12, 6);
    makeResizable (getWidth(), getHeight(), 2000, 1500);

    innerScope.setNumChannels (p.getTotalNumInputChannels());

    for (auto pp : scopeProc.getPluginParameters())
        pp->addListener (this);

    updateScope();
    updateTraceColours();
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    for (auto pp : scopeProc.getPluginParameters())
        pp->removeListener (this);
}

//==============================================================================

juce::Rectangle<int> PluginEditor::getGridArea (int x, int y, int w, int h)
{
    return juce::Rectangle<int> (getWidth() - inset - 2 * cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    if (isFullscreen)
    {
        // Fullscreen mode - scope takes entire area below header
        scope.setBounds (inset, headerHeight + inset, getWidth() - 2 * inset, getHeight() - headerHeight - 2 * inset);

        // Hide all controls
        for (auto* c : controls)
            c->setVisible (false);
    }
    else
    {
        // Normal mode
        scope.setBounds (inset, headerHeight + inset, getWidth() - 2 * cx - inset - 10, getHeight() - headerHeight - 2 * inset);

        // Show all controls
        for (auto* c : controls)
            c->setVisible (true);

        // Show either samples/pixel or beats depending on sync state
        bool syncOn = scopeProc.sync->isOn();
        componentForParam (*scopeProc.samplesPerPixel)->setVisible (! syncOn);
        componentForParam (*scopeProc.beatSync)->setVisible (syncOn);

        auto y = 0;
        {
            componentForParam (*scopeProc.sync)->setBounds (getGridArea (0, y));
            // Position both at same location - only one visible at a time
            componentForParam (*scopeProc.samplesPerPixel)->setBounds (getGridArea (1, y));
            componentForParam (*scopeProc.beatSync)->setBounds (getGridArea (1, y));
            y++;
        }

        {
            componentForParam (*scopeProc.triggerChannel)->setBounds (getGridArea (0, y));
            componentForParam (*scopeProc.triggerMode)->setBounds (getGridArea (1, y));
            y++;
        }

        {
            componentForParam (*scopeProc.triggerRun)->setBounds (getGridArea (0, y));
            componentForParam (*scopeProc.verticalZoom)->setBounds (getGridArea (1, y));
            y++;
        }

        {
            componentForParam (*scopeProc.verticalOffsetL)->setBounds (getGridArea (0, y));
            componentForParam (*scopeProc.verticalOffsetR)->setBounds (getGridArea (1, y));
            y++;
        }

        {
            componentForParam (*scopeProc.triggerLevel)->setBounds (getGridArea (0, y));
            componentForParam (*scopeProc.triggerPos)->setBounds (getGridArea (1, y));
            y++;
        }
    }

    // Toolbar buttons at bottom of scope
    auto buttonArea = scope.getBounds().removeFromBottom (20).removeFromLeft (280);
    buttonArea.translate (4, -4);
    playPauseButton.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    fullscreenButton.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (8);
    recordNormal.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordTriggered.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordRetrospective.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordStop.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordMenu.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (8);
    colourSwatch1.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    colourSwatch2.setBounds (buttonArea.removeFromLeft (20));
}

void PluginEditor::updateScope()
{
    auto& innerScope = scope.getScope();

    innerScope.setDrawTriggerPos (true);
    innerScope.setNumSamplesPerPixel (scopeProc.samplesPerPixel->getUserValue());
    innerScope.setSingleTrigger (scopeProc.triggerRun->isOn());
    innerScope.setVerticalZoomFactor (scopeProc.verticalZoom->getUserValue());
    innerScope.setVerticalZoomOffset (scopeProc.verticalOffsetL->getUserValue(), 0);
    innerScope.setVerticalZoomOffset (scopeProc.verticalOffsetR->getUserValue(), 1);
    innerScope.setTriggerChannel (int (scopeProc.triggerChannel->getUserValue()));
    innerScope.setTriggerMode ((gin::TriggeredScope::TriggerMode) int (scopeProc.triggerMode->getUserValue()));
    innerScope.setTriggerLevel (scopeProc.triggerLevel->getUserValue());
    innerScope.setTriggerPos (scopeProc.triggerPos->getUserValue());
    innerScope.setSampleRate (scopeProc.getSampleRate());

    // Beat sync: use beats value when sync is on, 0 when off
    bool syncOn = scopeProc.sync->isOn();
    innerScope.setBeatSync (syncOn ? int (scopeProc.beatSync->getUserValue()) : 0);

    // Update visibility of spp/beats controls (only when not fullscreen)
    if (! isFullscreen)
    {
        componentForParam (*scopeProc.samplesPerPixel)->setVisible (! syncOn);
        componentForParam (*scopeProc.beatSync)->setVisible (syncOn);
    }

    // For recording trigger, use at least -90dB threshold
    float minTriggerLevel = juce::Decibels::decibelsToGain (-90.0f);
    float recordTriggerLevel = std::max (std::abs (scopeProc.triggerLevel->getUserValue()), minTriggerLevel);
    scopeProc.audioRecorder.setTriggerLevel (recordTriggerLevel);

    updatePlayPauseButton();
}

void PluginEditor::updateTraceColours()
{
    auto& innerScope = scope.getScope();

    auto c1 = scopeProc.getTraceColour (0);
    auto c2 = scopeProc.getTraceColour (1);

    colourSwatch1.setColour (c1);
    colourSwatch2.setColour (c2);

    innerScope.setColour (gin::TriggeredScope::traceColourId + 0, c1);
    innerScope.setColour (gin::TriggeredScope::traceColourId + 1, c2);
    innerScope.setColour (gin::TriggeredScope::envelopeColourId + 0, c1);
    innerScope.setColour (gin::TriggeredScope::envelopeColourId + 1, c2);

    innerScope.repaint();
}

void PluginEditor::timerCallback()
{
    flashState = ! flashState;
    updateRecordButtonColours();
}

void PluginEditor::updateRecordButtons()
{
    const bool isRecording = scopeProc.audioRecorder.isRecording();
    const bool isWaiting = scopeProc.audioRecorder.isWaitingForTrigger();

    recordNormal.setEnabled (! isWaiting);
    recordTriggered.setEnabled (! isRecording || isWaiting);
    recordRetrospective.setEnabled (! isRecording && ! isWaiting);
    recordStop.setEnabled (isRecording || isWaiting);
}

void PluginEditor::updateRecordButtonColours()
{
    const auto mode = scopeProc.audioRecorder.getRecordMode();
    auto accentColour = findColour (gin::GinLookAndFeel::accentColourId);

    if (mode == AudioRecorder::RecordMode::normal)
        recordNormal.setColour (accentColour);
    else
        recordNormal.clearColour();

    if (mode == AudioRecorder::RecordMode::waitingForTrigger)
    {
        if (flashState)
            recordTriggered.setColour (accentColour);
        else
            recordTriggered.clearColour();
    }
    else if (mode == AudioRecorder::RecordMode::triggered)
        recordTriggered.setColour (accentColour);
    else
        recordTriggered.clearColour();
}

void PluginEditor::updatePlayPauseButton()
{
    auto& innerScope = scope.getScope();
    bool isSingleMode = scopeProc.triggerRun->isOn();
    bool isPaused = innerScope.isPaused();

    // Show play icon when paused or in single mode (waiting/frozen)
    // Show pause icon when running normally
    bool showPlay = isPaused || isSingleMode;
    playPauseButton.rawSVG = showPlay ? playSVG : pauseSVG;

    auto accentColour = findColour (gin::GinLookAndFeel::accentColourId);
    if (showPlay)
        playPauseButton.setColour (accentColour);
    else
        playPauseButton.clearColour();

    playPauseButton.repaint();
}

void PluginEditor::setFullscreen (bool fs)
{
    isFullscreen = fs;
    fullscreenButton.rawSVG = isFullscreen ? exitFullscreenSVG : fullscreenSVG;

    auto accentColour = findColour (gin::GinLookAndFeel::accentColourId);
    if (isFullscreen)
        fullscreenButton.setColour (accentColour);
    else
        fullscreenButton.clearColour();

    fullscreenButton.repaint();
    resized();
}

void PluginEditor::showRecordMenu()
{
    juce::PopupMenu menu;

    menu.addItem ("Set Destination Folder...", [this]
    {
        auto fc = std::make_shared<juce::FileChooser> ("Select Destination Folder",
            scopeProc.audioRecorder.getDestFolder());

        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        fc->launchAsync (flags, [this, fc] (const juce::FileChooser&)
        {
            auto folder = fc->getResult();
            if (folder.isDirectory())
                scopeProc.audioRecorder.setDestFolder (folder);
        });
    });

    menu.addSeparator();

    auto destFolder = scopeProc.audioRecorder.getDestFolder();
    menu.addItem ("Open Destination Folder", destFolder.isDirectory(), false, [destFolder]
    {
        destFolder.revealToUser();
    });

    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (recordMenu));
}
