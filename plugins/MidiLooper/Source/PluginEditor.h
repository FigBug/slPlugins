#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class MidiFileComponent : public juce::Component,
                          public juce::FileDragAndDropTarget,
                          public juce::Timer
{
public:
    MidiFileComponent (gin::MidiFilePlayer& player) : midiPlayer (player)
    {
        startTimerHz (30);
    }

    ~MidiFileComponent() override
    {
        stopTimer();
    }

    void timerCallback() override
    {
        const bool isPlaying = midiPlayer.isPlaying();

        if (wasPlaying && ! isPlaying)
        {
            if (onPlaybackStateChanged)
                onPlaybackStateChanged();
        }

        if (isPlaying || wasPlaying != isPlaying)
            repaint();

        wasPlaying = isPlaying;
    }

    bool isInterestedInFileDrag (const juce::StringArray& files) override
    {
        for (const auto& file : files)
        {
            if (juce::File (file).hasFileExtension (".mid;.midi"))
                return true;
        }
        return false;
    }

    void fileDragEnter (const juce::StringArray&, int, int) override
    {
        isDragOver = true;
        repaint();
    }

    void fileDragExit (const juce::StringArray&) override
    {
        isDragOver = false;
        repaint();
    }

    void filesDropped (const juce::StringArray& files, int, int) override
    {
        isDragOver = false;

        for (const auto& file : files)
        {
            juce::File f (file);
            if (f.hasFileExtension (".mid;.midi"))
            {
                midiPlayer.load (f);
                if (onFileDropped)
                    onFileDropped();
                break;
            }
        }

        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto rc = getLocalBounds();

        g.setColour (findColour (gin::GinLookAndFeel::displayColourId));
        g.fillRect (rc);

        if (midiPlayer.hasFileLoaded())
        {
            g.setColour (findColour (gin::GinLookAndFeel::accentColourId));

            auto fileName = midiPlayer.getLoadedFile().getFileName();
            g.drawText (fileName, rc.reduced (4), juce::Justification::centredLeft);

            // Draw playhead position
            if (midiPlayer.getLengthInSeconds() > 0.0)
            {
                double pos = midiPlayer.getPlayheadPosition() / midiPlayer.getLengthInSeconds();
                int x = static_cast<int> (pos * rc.getWidth());
                g.setColour (findColour (gin::GinLookAndFeel::accentColourId).withAlpha (0.5f));
                g.drawVerticalLine (x, static_cast<float> (rc.getY()), static_cast<float> (rc.getBottom()));
            }
        }
        else
        {
            g.setColour (findColour (gin::GinLookAndFeel::grey45ColourId));
            g.drawText ("Drop MIDI file here", rc, juce::Justification::centred);
        }

        if (isDragOver)
        {
            g.setColour (findColour (gin::GinLookAndFeel::accentColourId).withAlpha (0.2f));
            g.fillAll();
        }

        g.setColour (findColour (gin::GinLookAndFeel::grey45ColourId));
        g.drawRect (rc);
    }

    std::function<void()> onFileDropped;
    std::function<void()> onPlaybackStateChanged;

private:
    gin::MidiFilePlayer& midiPlayer;
    bool isDragOver = false;
    bool wasPlaying = false;
};

//==============================================================================
class MidiLooperAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    MidiLooperAudioProcessorEditor (MidiLooperAudioProcessor&);
    ~MidiLooperAudioProcessorEditor() override;

    void resized() override;

private:
    void updatePlayStopButtons();
    void showMenu();

    MidiLooperAudioProcessor& proc;

    MidiFileComponent   midiFile { proc.midiPlayer };
    gin::SVGButton      midiPlay;
    gin::SVGButton      midiStop;
    gin::SVGButton      midiMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLooperAudioProcessorEditor)
};
