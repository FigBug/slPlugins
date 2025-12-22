#include "PitchScope.h"
#include "PluginProcessor.h"

PitchScope::PitchScope (gin::AudioFifo& fifo, PluginProcessor& proc)
    : scope (fifo), processor (proc)
{
    addAndMakeVisible (scope);
    startTimerHz (10);
}

PitchScope::~PitchScope()
{
    stopTimer();
}

void PitchScope::timerCallback()
{
    float pitch = processor.getDetectedPitch();
    if (std::abs (pitch - lastPitch) > 0.5f)
    {
        lastPitch = pitch;
        repaint();
    }
}

void PitchScope::resized()
{
    scope.setBounds (getLocalBounds());
}

void PitchScope::paint (juce::Graphics& g)
{
    // Draw dB grid lines based on channel 0 zoom/offset
    float zoom = processor.verticalZoom->getUserValue();
    float offset = processor.verticalOffsetL->getUserValue();
    float h = float (getHeight());
    float w = float (getWidth());
    float centre = h / 2.0f;

    // Convert amplitude to Y position
    auto ampToY = [&] (float amp) -> float
    {
        return centre - (amp * zoom + offset) * centre;
    };

    g.setColour (juce::Colours::white.withAlpha (0.15f));
    auto font = g.getCurrentFont().withHeight (10.0f);
    g.setFont (font);

    // dB levels to draw (both positive and negative amplitude)
    const float dbLevels[] = { 0.0f, -3.0f, -6.0f, -10.0f, -20.0f };

    for (float db : dbLevels)
    {
        float amp = juce::Decibels::decibelsToGain (db);

        // Draw line for positive amplitude
        float yPos = ampToY (amp);
        if (yPos >= 0 && yPos <= h)
        {
            g.drawHorizontalLine (int (yPos), 0.0f, w);
            juce::String label = (db == 0.0f) ? "0 dB" : juce::String (int (db)) + " dB";
            g.drawText (label, 4, int (yPos) - 12, 40, 12, juce::Justification::bottomLeft);
        }

        // Draw line for negative amplitude (mirror)
        if (db < 0.0f)
        {
            float yNeg = ampToY (-amp);
            if (yNeg >= 0 && yNeg <= h)
            {
                g.drawHorizontalLine (int (yNeg), 0.0f, w);
                g.drawText (juce::String (int (db)) + " dB", 4, int (yNeg), 40, 12, juce::Justification::topLeft);
            }
        }
    }

    // Draw pitch info (uses lastPitch which is cleared after timeout)
    if (lastPitch > 20.0f && lastPitch < 20000.0f)
    {
        int midiNote = frequencyToMidiNote (lastPitch);
        juce::String noteStr = midiNoteToString (midiNote);

        // Calculate cents deviation
        float exactNote = 12.0f * std::log2 (lastPitch / 440.0f) + 69.0f;
        int cents = juce::roundToInt ((exactNote - float (midiNote)) * 100.0f);
        juce::String centsStr = (cents >= 0 ? "+" : "") + juce::String (cents);

        juce::String text = juce::String (lastPitch, 1) + " Hz  " + noteStr + " (" + centsStr + ")";

        juce::GlyphArrangement glyphs;
        glyphs.addLineOfText (g.getCurrentFont(), text, 0.0f, 0.0f);
        float textWidth = glyphs.getBoundingBox (0, -1, true).getWidth() + 12.0f;
        float textX = w - textWidth - 4.0f;
        float textY = h - 20.0f;

        g.setColour (juce::Colours::black.withAlpha (0.7f));
        g.fillRoundedRectangle (textX, textY, textWidth, 16.0f, 3.0f);
        g.setColour (juce::Colours::white);
        g.drawText (text, juce::Rectangle<float> (textX, textY, textWidth, 16.0f), juce::Justification::centred);
    }
}

int PitchScope::frequencyToMidiNote (float frequency)
{
    return juce::roundToInt (12.0f * std::log2 (frequency / 440.0f) + 69.0f);
}

juce::String PitchScope::midiNoteToString (int noteNumber)
{
    static const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    int octave = (noteNumber / 12) - 1;
    int noteIndex = noteNumber % 12;

    return juce::String (noteNames[noteIndex]) + juce::String (octave);
}
