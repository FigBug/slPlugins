#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MathsAudioProcessorEditor : public gin::ProcessorEditor,
                                  private juce::TextEditor::Listener
{
public:
    MathsAudioProcessorEditor (MathsAudioProcessor&);
    ~MathsAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void refresh();

private:
    void textEditorReturnKeyPressed (juce::TextEditor&) override;
    void textEditorEscapeKeyPressed (juce::TextEditor&) override;
    void textEditorFocusLost (juce::TextEditor&) override;
    
    MathsAudioProcessor& mathsProcessor;
    juce::TextEditor l, r, a, b;
    juce::Label lLabel { "", "L out:" };
    juce::Label rLabel { "", "R out:" };
    juce::Label aLabel { "", "A var:" };
    juce::Label bLabel { "", "B var:" };
    juce::Label errors;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MathsAudioProcessorEditor)
};
