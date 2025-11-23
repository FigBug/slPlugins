#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MathsAudioProcessorEditor::MathsAudioProcessorEditor (MathsAudioProcessor& p)
    : gin::ProcessorEditor (p), mathsProcessor (p)
{
    juce::Font f (juce::Font::getDefaultMonospacedFontName(), 12, juce::Font::plain);

    l.setFont (f);
    r.setFont (f);
    a.setFont (f);
    b.setFont (f);

    addAndMakeVisible (l);
    addAndMakeVisible (r);
    addAndMakeVisible (a);
    addAndMakeVisible (b);
    l.setJustification (juce::Justification::centredLeft);
    r.setJustification (juce::Justification::centredLeft);
    a.setJustification (juce::Justification::centredLeft);
    b.setJustification (juce::Justification::centredLeft);
    addAndMakeVisible (lLabel);
    addAndMakeVisible (rLabel);
    addAndMakeVisible (aLabel);
    addAndMakeVisible (bLabel);
    addAndMakeVisible (errors);

    l.addListener (this);
    r.addListener (this);
    a.addListener (this);
    b.addListener (this);

    l.setText (p.lEquation, juce::dontSendNotification);
    r.setText (p.rEquation, juce::dontSendNotification);
    a.setText (p.aEquation, juce::dontSendNotification);
    b.setText (p.bEquation, juce::dontSendNotification);

    errors.setJustificationType (juce::Justification::centred);

    int count = 0;
    for (auto pp : p.getPluginParameters())
    {
        if (count == 4)
        {
            auto pc = new gin::Switch (pp);
            addAndMakeVisible (pc);
            controls.add (pc);
        }
        else
        {
            auto pc = new gin::Knob (pp, count == 2 || count == 3);
            addAndMakeVisible (pc);
            controls.add (pc);
        }
        
        count++;
    }
    
    setGridSize (9, 3);
    setSize (getWidth(), getHeight() + 20);
}

MathsAudioProcessorEditor::~MathsAudioProcessorEditor()
{
}

//==============================================================================
void MathsAudioProcessorEditor::refresh()
{
    l.setText (mathsProcessor.lEquation, juce::dontSendNotification);
    r.setText (mathsProcessor.rEquation, juce::dontSendNotification);
    a.setText (mathsProcessor.aEquation, juce::dontSendNotification);
    b.setText (mathsProcessor.bEquation, juce::dontSendNotification);

    juce::String err;
    auto doErr = [&] (juce::String title, juce::String msg)
    {
        if (msg.isNotEmpty())
            err += title + " " + msg + " ";
    };
    
    doErr ("L:", mathsProcessor.lError);
    doErr ("R:", mathsProcessor.rError);
    doErr ("A:", mathsProcessor.aError);
    doErr ("B:", mathsProcessor.bError);

    errors.setText (err, juce::dontSendNotification);
}

void MathsAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();
    
    componentForId (PARAM_P1)->setBounds (getGridArea (0, 2));
    componentForId (PARAM_P2)->setBounds (getGridArea (1, 2));
    componentForId (PARAM_P3)->setBounds (getGridArea (2, 2));
    componentForId (PARAM_P4)->setBounds (getGridArea (3, 2));
    componentForId (PARAM_LIMITER)->setBounds (getGridArea (8, 2));
    
    auto rc = getGridArea (0, 0, 9, 2);
    rc = rc.withSizeKeepingCentre (rc.getWidth() - 4, rc.getHeight());
    
    int h = rc.getHeight() / 4;
    auto rcLabels = rc.removeFromLeft (50);
    aLabel.setBounds (rcLabels.removeFromTop (h));
    bLabel.setBounds (rcLabels.removeFromTop (h));
    lLabel.setBounds (rcLabels.removeFromTop (h));
    rLabel.setBounds (rcLabels.removeFromTop (h));

    a.setBounds (rc.removeFromTop (h).reduced (0, 2));
    b.setBounds (rc.removeFromTop (h).reduced (0, 2));
    l.setBounds (rc.removeFromTop (h).reduced (0, 2));
    r.setBounds (rc.removeFromTop (h).reduced (0, 2));

    auto rcError = getLocalBounds().removeFromBottom (20);
    errors.setBounds (rcError.reduced (4));
}

void MathsAudioProcessorEditor::textEditorReturnKeyPressed (juce::TextEditor& ed)
{
    if (&ed == &l)
    {
        mathsProcessor.lEquation = ed.getText();
        mathsProcessor.setupParsers();
        refresh();
    }
    else if (&ed == &r)
    {
        mathsProcessor.rEquation = ed.getText();
        mathsProcessor.setupParsers();
        refresh();
    }
    else if (&ed == &a)
    {
        mathsProcessor.aEquation = ed.getText();
        mathsProcessor.setupParsers();
        refresh();
    }
    else if (&ed == &b)
    {
        mathsProcessor.bEquation = ed.getText();
        mathsProcessor.setupParsers();
        refresh();
    }
}

void MathsAudioProcessorEditor::textEditorEscapeKeyPressed (juce::TextEditor& ed)
{
    if (&ed == &l)
        l.setText (mathsProcessor.lEquation, juce::dontSendNotification);
    else if (&ed == &r)
        r.setText (mathsProcessor.rEquation, juce::dontSendNotification);
    else if (&ed == &r)
        a.setText (mathsProcessor.aEquation, juce::dontSendNotification);
    else if (&ed == &r)
        b.setText (mathsProcessor.bEquation, juce::dontSendNotification);
}

void MathsAudioProcessorEditor::textEditorFocusLost (juce::TextEditor& ed)
{
    textEditorReturnKeyPressed (ed);
}

