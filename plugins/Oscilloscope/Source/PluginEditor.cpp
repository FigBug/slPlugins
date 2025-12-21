#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    const char* recordSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><circle cx="12" cy="12" r="8" fill="currentColor"/></svg>)";
    const char* triggerSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path fill-rule="evenodd" d="M12 4a8 8 0 1 0 0 16 8 8 0 0 0 0-16zM8 8h8v2h-3v7h-2v-7H8V8z" fill="currentColor"/></svg>)";
    const char* retroSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><path fill-rule="evenodd" d="M12 4a8 8 0 1 0 0 16 8 8 0 0 0 0-16zM9 7h5a3 3 0 0 1 .5 5.96L17 17h-2.5l-2.3-4H11v4H9V7zm2 2v2h3a1 1 0 1 0 0-2h-3z" fill="currentColor"/></svg>)";
    const char* stopSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2" ry="2" fill="currentColor"/></svg>)";
    const char* menuSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="4" y="6" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="11" width="16" height="2" rx="1" fill="currentColor"/><rect x="4" y="16" width="16" height="2" rx="1" fill="currentColor"/></svg>)";
}

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), scopeProc (p),
      recordNormal ("record", recordSVG, 4),
      recordTriggered ("trigger", triggerSVG, 4),
      recordRetrospective ("retro", retroSVG, 4),
      recordStop ("stop", stopSVG, 4),
      recordMenu ("menu", menuSVG, 4)
{
    addAndMakeVisible (&scope);

    auto c1 = juce::Colours::white.overlaidWith (juce::Colours::blue.withAlpha (0.3f));
    auto c2 = juce::Colours::white.overlaidWith (juce::Colours::yellow.withAlpha (0.3f));

    scope.setColour (gin::TriggeredScope::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));

    scope.setColour (gin::TriggeredScope::traceColourId + 0, c1);
    scope.setColour (gin::TriggeredScope::traceColourId + 1, c2);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 0, c1);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 1, c2);

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

    scopeProc.audioRecorder.onRecordingStarted = [this] { updateRecordButtons(); updateRecordButtonColours(); };
    scopeProc.audioRecorder.onRecordingStopped = [this] { updateRecordButtons(); updateRecordButtonColours(); };

    startTimerHz (6);
    updateRecordButtons();

    setGridSize (8, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);

    scope.setNumChannels (p.getTotalNumInputChannels());

    for (auto pp : scopeProc.getPluginParameters())
        pp->addListener (this);

    updateScope();
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
    return juce::Rectangle<int> (getWidth() - inset - 3 * cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    scope.setBounds (inset, headerHeight + inset, getWidth() - 3 * cx - inset - 10, getHeight() - headerHeight - 2 * inset);

    componentForParam (*scopeProc.samplesPerPixel)->setBounds (getGridArea (0, 0));
    componentForParam (*scopeProc.verticalZoom)->setBounds (getGridArea (0, 1));
    componentForParam (*scopeProc.verticalOffsetL)->setBounds (getGridArea (0, 2));
    componentForParam (*scopeProc.verticalOffsetR)->setBounds (getGridArea (0, 3));

    componentForParam (*scopeProc.triggerChannel)->setBounds (getGridArea (1, 0));
    componentForParam (*scopeProc.triggerMode)->setBounds (getGridArea (2, 0));
    componentForParam (*scopeProc.triggerRun)->setBounds (getGridArea (1, 1));
    componentForParam (*scopeProc.triggerReset)->setBounds (getGridArea (2, 1));

    componentForParam (*scopeProc.triggerLevel)->setBounds (getGridArea (1, 3));
    componentForParam (*scopeProc.triggerPos)->setBounds (getGridArea (2, 3));

    // Recording controls at bottom of scope
    auto buttonArea = scope.getBounds().removeFromBottom (20).removeFromLeft (120);
    buttonArea.translate (4, -4);
    recordNormal.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordTriggered.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordRetrospective.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordStop.setBounds (buttonArea.removeFromLeft (20));
    buttonArea.removeFromLeft (4);
    recordMenu.setBounds (buttonArea.removeFromLeft (20));
}

void PluginEditor::updateScope()
{
    scope.setDrawTriggerPos (true);
    scope.setNumSamplesPerPixel (int (scopeProc.samplesPerPixel->getUserValue()));
    scope.setSingleTrigger (scopeProc.triggerRun->isOn());
    scope.setVerticalZoomFactor (scopeProc.verticalZoom->getUserValue());
    scope.setVerticalZoomOffset (scopeProc.verticalOffsetL->getUserValue(), 0);
    scope.setVerticalZoomOffset (scopeProc.verticalOffsetR->getUserValue(), 1);
    scope.setTriggerChannel (int (scopeProc.triggerChannel->getUserValue()));
    scope.setTriggerMode ((gin::TriggeredScope::TriggerMode) int (scopeProc.triggerMode->getUserValue()));
    scope.setTriggerLevel (scopeProc.triggerLevel->getUserValue());
    scope.setTriggerPos (scopeProc.triggerPos->getUserValue());

    scopeProc.audioRecorder.setTriggerLevel (scopeProc.triggerLevel->getUserValue());

    if (scopeProc.triggerReset->isOn())
    {
        scope.resetTrigger();
        juce::Timer::callAfterDelay (50, [this] { scopeProc.triggerReset->setValueNotifyingHost (0.0f); });
    }
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
