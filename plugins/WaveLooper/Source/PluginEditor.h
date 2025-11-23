#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class Waveform : public gin::WaveformComponent,
                 public juce::FileDragAndDropTarget
{
public:
    Waveform (WaveLooperAudioProcessor& p) : proc (p)
    {
    }
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override
    {
        return true;
    }

    void fileDragEnter (const juce::StringArray&, int, int) override
    {
        dragOver = true;
        repaint();
    }

    void fileDragMove (const juce::StringArray&, int, int) override
    {
        dragOver = true;
        repaint();
    }

    void fileDragExit (const juce::StringArray& files) override
    {
        dragOver = false;
        repaint();
    }

    void filesDropped (const juce::StringArray& files, int x, int y) override
    {
        proc.setFile (files[0]);
        
        dragOver = false;
        repaint();
    }
    
    void paint (juce::Graphics& g) override
    {
        gin::WaveformComponent::paint (g);

        if (dragOver)
        {
            g.setColour (juce::Colours::white.withAlpha (0.3f));
            g.fillRect (getLocalBounds());
        }
    }
    
    WaveLooperAudioProcessor& proc;
    bool dragOver = false;
};

//==============================================================================
/**
*/
class WaveLooperAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor&);
    ~WaveLooperAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void refresh();

private:    
    WaveLooperAudioProcessor& proc;
    
    Waveform thumbnail {proc};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessorEditor)
};
