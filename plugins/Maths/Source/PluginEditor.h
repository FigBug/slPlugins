#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MathsAudioProcessorEditor : public gin::ProcessorEditor,
                                  private TextEditor::Listener
{
public:
    MathsAudioProcessorEditor (MathsAudioProcessor&);
    ~MathsAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void refresh();

private:
    void textEditorReturnKeyPressed (TextEditor&) override;
    void textEditorEscapeKeyPressed (TextEditor&) override;
    void textEditorFocusLost (TextEditor&) override;
    
    MathsAudioProcessor& mathsProcessor;
    TextEditor l, r, a, b;
    Label lLabel { "", "L out:" };
    Label rLabel { "", "R out:" };
    Label aLabel { "", "A var:" };
    Label bLabel { "", "B var:" };
    Label errors;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MathsAudioProcessorEditor)
};
