#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class SampleFileComponent : public gin::AudioSamplerBufferComponent,
                            public juce::FileDragAndDropTarget,
                            public juce::Timer
{
public:
    SampleFileComponent (gin::SamplePlayer& player) : samplePlayer (player)
    {
        startTimerHz (30);
    }

    ~SampleFileComponent() override
    {
        stopTimer();
    }

    void timerCallback() override
    {
        const bool isPlaying = samplePlayer.isPlaying();

        if (wasPlaying && ! isPlaying)
        {
            if (onPlaybackStateChanged)
                onPlaybackStateChanged();
        }

        if (samplePlayer.hasFileLoaded())
        {
            const double pos = samplePlayer.getPosition();
            setPlayheads ({ static_cast<int> (pos) });
        }

        if (isPlaying || wasPlaying != isPlaying)
            repaint();

        wasPlaying = isPlaying;
    }

    bool isInterestedInFileDrag (const juce::StringArray& files) override
    {
        juce::AudioFormatManager mgr;
        mgr.registerBasicFormats();

        for (const auto& file : files)
        {
            if (mgr.findFormatForFileExtension (juce::File (file).getFileExtension()) != nullptr)
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

        juce::AudioFormatManager mgr;
        mgr.registerBasicFormats();

        for (const auto& file : files)
        {
            juce::File f (file);
            if (mgr.findFormatForFileExtension (f.getFileExtension()) != nullptr)
            {
                samplePlayer.load (f);
                setBuffer (samplePlayer.getBuffer());
                if (onFileDropped)
                    onFileDropped();
                break;
            }
        }

        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        gin::AudioSamplerBufferComponent::paint (g);

        if (! samplePlayer.hasFileLoaded())
        {
            g.setColour (findColour (gin::GinLookAndFeel::grey45ColourId));
            g.drawText ("Drop audio file here", getLocalBounds(), juce::Justification::centred);
        }

        if (isDragOver)
        {
            g.setColour (findColour (gin::GinLookAndFeel::accentColourId).withAlpha (0.2f));
            g.fillAll();
        }

        g.setColour (findColour (gin::GinLookAndFeel::grey45ColourId));
        g.drawRect (getLocalBounds());
    }

    std::function<void()> onFileDropped;
    std::function<void()> onPlaybackStateChanged;

private:
    gin::SamplePlayer& samplePlayer;
    bool isDragOver = false;
    bool wasPlaying = false;
};

//==============================================================================
class WaveLooperAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor&);
    ~WaveLooperAudioProcessorEditor() override;

    void resized() override;

private:
    void updatePlayStopButtons();
    void showMenu();

    WaveLooperAudioProcessor& proc;

    SampleFileComponent sample { proc.samplePlayer };
    gin::SVGButton      samplePlay;
    gin::SVGButton      sampleStop;
    gin::SVGButton      sampleMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessorEditor)
};
