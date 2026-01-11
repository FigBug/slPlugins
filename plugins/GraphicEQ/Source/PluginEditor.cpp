#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// FilterResponseCurve implementation
//==============================================================================
FilterResponseCurve::FilterResponseCurve (GraphicEQAudioProcessor& p)
    : proc (p)
{
    setInterceptsMouseClicks (false, false);
    startTimerHz (30);
}

FilterResponseCurve::~FilterResponseCurve()
{
}

void FilterResponseCurve::timerCallback()
{
    repaint();
}

float FilterResponseCurve::frequencyToX (float freq) const
{
    auto minFreq = 20.0f;
    auto maxFreq = 20000.0f;
    auto normX = (std::log (freq / minFreq)) / (std::log (maxFreq / minFreq));
    return normX * getWidth();
}

float FilterResponseCurve::dbToY (float db) const
{
    auto minDb = -24.0f;
    auto maxDb = 24.0f;
    auto normY = 1.0f - (db - minDb) / (maxDb - minDb);
    return normY * getHeight();
}

void FilterResponseCurve::paint (juce::Graphics& g)
{
    juce::Path responsePath;

    auto minFreq = 20.0f;
    auto maxFreq = 20000.0f;

    bool firstPoint = true;

    for (float x = 0; x < getWidth(); x += 2.0f)
    {
        auto normX = x / getWidth();
        auto freq = minFreq * std::pow (maxFreq / minFreq, normX);

        // Get EQ response (use channel 0 since both are the same)
        float response = proc.eq.getFrequencyResponse (0, freq);

        // Add HP filter response if enabled
        if (proc.hpEnable->getUserValue() > 0.5f)
            response *= proc.hpFilter.getResponseMagnitude (freq);

        // Add LP filter response if enabled
        if (proc.lpEnable->getUserValue() > 0.5f)
            response *= proc.lpFilter.getResponseMagnitude (freq);

        auto db = juce::Decibels::gainToDecibels (response);
        db = juce::jlimit (-24.0f, 24.0f, db);

        auto y = dbToY (db);

        if (firstPoint)
        {
            responsePath.startNewSubPath (x, y);
            firstPoint = false;
        }
        else
        {
            responsePath.lineTo (x, y);
        }
    }

    // Draw the response curve
    g.setColour (findColour (gin::PluginLookAndFeel::accentColourId));
    g.strokePath (responsePath, juce::PathStrokeType (2.0f));

    // Draw filled area under curve
    auto filledPath = responsePath;
    filledPath.lineTo ((float) getWidth(), (float) getHeight() / 2.0f);
    filledPath.lineTo (0.0f, (float) getHeight() / 2.0f);
    filledPath.closeSubPath();

    g.setColour (findColour (gin::PluginLookAndFeel::accentColourId).withAlpha (0.15f));
    g.fillPath (filledPath);
}

//==============================================================================
// Editor implementation
//==============================================================================
GraphicEQAudioProcessorEditor::GraphicEQAudioProcessorEditor (GraphicEQAudioProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    // Create band faders
    for (int i = 0; i < GraphicEQAudioProcessor::numBands; i++)
    {
        auto* fader = new gin::Fader (proc.bandGain[static_cast<size_t> (i)]);
        addAndMakeVisible (fader);
        bandFaders.add (fader);
    }

    // HP filter controls
    addAndMakeVisible (hpEnableButton);
    addAndMakeVisible (hpFreqKnob);
    addAndMakeVisible (hpSlopeButton);

    // LP filter controls
    addAndMakeVisible (lpEnableButton);
    addAndMakeVisible (lpFreqKnob);
    addAndMakeVisible (lpSlopeButton);

    // Make slope button text color the same for on and off states
    auto slopeTextColour = findColour (juce::TextButton::textColourOnId);
    hpSlopeButton.setColour (juce::TextButton::textColourOffId, slopeTextColour);
    lpSlopeButton.setColour (juce::TextButton::textColourOffId, slopeTextColour);

    // Output gain
    addAndMakeVisible (outputFader);

    // Spectrum analyzer
    addAndMakeVisible (spectrum);
    spectrum.setNumChannels (2);
    spectrum.setDrawGrid (true);
    spectrum.setFrequencyRange (20.0f, 20000.0f);
    spectrum.setDbRange (-60.0f, 6.0f);
    spectrum.setSampleRate (p.getSampleRate());
    spectrum.setColour (gin::SpectrumAnalyzer::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));
    spectrum.setColour (gin::SpectrumAnalyzer::gridColourId, findColour (gin::PluginLookAndFeel::grey45ColourId).withMultipliedAlpha (0.2f));
    spectrum.setColour (gin::SpectrumAnalyzer::backgroundColourId, juce::Colours::transparentBlack);

    auto accent = findColour (gin::PluginLookAndFeel::accentColourId);
    for (int i = 0; i < 2; ++i)
    {
        auto c = accent.withRotatedHue (-0.05f + i * 0.1f);
        spectrum.setColour (gin::SpectrumAnalyzer::traceColourId + i, c);
        spectrum.setColour (gin::SpectrumAnalyzer::envelopeColourId + i, c.withAlpha (0.5f));
    }

    // Response curve overlay
    addAndMakeVisible (responseCurve);

    // Listen to enable parameters
    proc.lpEnable->addListener (this);
    proc.hpEnable->addListener (this);

    updateFilterEnableStates();

    setGridSize (14, 4);
}

GraphicEQAudioProcessorEditor::~GraphicEQAudioProcessorEditor()
{
    proc.lpEnable->removeListener (this);
    proc.hpEnable->removeListener (this);
}

void GraphicEQAudioProcessorEditor::valueUpdated (gin::Parameter* param)
{
    if (param == proc.lpEnable || param == proc.hpEnable)
        updateFilterEnableStates();
}

void GraphicEQAudioProcessorEditor::updateFilterEnableStates()
{
    bool lpEnabled = proc.lpEnable->getUserValue() > 0.5f;
    lpFreqKnob.setEnabled (lpEnabled);
    lpSlopeButton.setEnabled (lpEnabled);

    bool hpEnabled = proc.hpEnable->getUserValue() > 0.5f;
    hpFreqKnob.setEnabled (hpEnabled);
    hpSlopeButton.setEnabled (hpEnabled);
}

void GraphicEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    gin::ProcessorEditor::paint (g);
}

void GraphicEQAudioProcessorEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto area = getFullGridArea();

    // Top row: HP controls, band faders, LP controls, output
    auto topRow = area.removeFromTop (juce::roundToInt (area.getHeight() * 0.55f));

    // HP controls on left (enable 25%, knob 50%, slope 25%)
    auto hpArea = topRow.removeFromLeft (56);
    auto hpHeight = hpArea.getHeight();
    hpEnableButton.setBounds (hpArea.removeFromTop (juce::roundToInt (hpHeight * 0.25f)));
    hpSlopeButton.setBounds (hpArea.removeFromBottom (juce::roundToInt (hpHeight * 0.25f)));
    hpFreqKnob.setBounds (hpArea);

    // LP controls on right (enable 25%, knob 50%, slope 25%)
    auto lpArea = topRow.removeFromRight (56);
    auto lpHeight = lpArea.getHeight();
    lpEnableButton.setBounds (lpArea.removeFromTop (juce::roundToInt (lpHeight * 0.25f)));
    lpSlopeButton.setBounds (lpArea.removeFromBottom (juce::roundToInt (lpHeight * 0.25f)));
    lpFreqKnob.setBounds (lpArea);

    // Band faders + output using Grid
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    juce::Grid faderGrid;
    faderGrid.templateRows = { Track (Fr (1)) };

    // Add columns for each band fader
    for (int i = 0; i < GraphicEQAudioProcessor::numBands; i++)
        faderGrid.templateColumns.add (Track (Fr (1)));

    // Add 10px gap before output
    faderGrid.templateColumns.add (Track (Px (10)));
    // Add output column
    faderGrid.templateColumns.add (Track (Fr (1)));

    // Add items
    for (int i = 0; i < GraphicEQAudioProcessor::numBands; i++)
        faderGrid.items.add (juce::GridItem (bandFaders[i]));

    // Empty item for gap
    faderGrid.items.add (juce::GridItem());
    // Output fader
    faderGrid.items.add (juce::GridItem (outputFader));

    faderGrid.performLayout (topRow);

    // Bottom: spectrum analyzer with response curve overlay
    auto spectrumArea = area.reduced (4);
    spectrum.setBounds (spectrumArea);
    responseCurve.setBounds (spectrumArea);
}
