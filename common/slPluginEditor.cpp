#include "slPluginEditor.h"
#include "slLookAndFeel.h"

slAudioProcessorEditor::slAudioProcessorEditor (slProcessor& p) noexcept
  : AudioProcessorEditor (p)
{
    setLookAndFeel (slLookAndFeel::getInstance());
}

slAudioProcessorEditor::slAudioProcessorEditor (slProcessor* p) noexcept
  : AudioProcessorEditor (p)
{
    setLookAndFeel (slLookAndFeel::getInstance());
}

void slAudioProcessorEditor::paint (Graphics& g)
{
    if (slLookAndFeel* lf = dynamic_cast<slLookAndFeel*> (&getLookAndFeel()))
    {
        g.fillAll (lf->veryPaleYellow);
    }
}

void slAudioProcessorEditor::resized()
{
    
}

Rectangle<int> slAudioProcessorEditor::getControlsArea()
{
    return getLocalBounds().withTrimmedTop (20);
}
