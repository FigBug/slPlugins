#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
class Waveform : public gin::WaveformComponent,
                 public FileDragAndDropTarget
{
public:
    Waveform (WaveLooperAudioProcessor& p) : proc (p)
    {
    }
    
    bool isInterestedInFileDrag (const StringArray& files) override
    {
        return true;
    }

    void fileDragEnter (const StringArray&, int, int) override
    {
        dragOver = true;
        repaint();
    }

    void fileDragMove (const StringArray&, int, int) override
    {
        dragOver = true;
        repaint();
    }

    void fileDragExit (const StringArray& files) override
    {
        dragOver = false;
        repaint();
    }

    void filesDropped (const StringArray& files, int x, int y) override
    {
        proc.setFile (files[0]);
        
        dragOver = false;
        repaint();
    }
    
    void paint (Graphics& g) override
    {
        gin::WaveformComponent::paint (g);
        
        if (dragOver)
        {
            g.setColour (Colours::white.withAlpha (0.3f));
            g.fillRect (getLocalBounds());
        }
    }
    
    WaveLooperAudioProcessor& proc;
    bool dragOver = false;
};

//==============================================================================
/**
*/
class WaveLooperAudioProcessorEditor : public gin::GinAudioProcessorEditor
{
public:
    WaveLooperAudioProcessorEditor (WaveLooperAudioProcessor&);
    ~WaveLooperAudioProcessorEditor();

    //==============================================================================
    void resized() override;
    void refresh();

private:    
    WaveLooperAudioProcessor& proc;
    
    Waveform thumbnail {proc};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveLooperAudioProcessorEditor)
};
