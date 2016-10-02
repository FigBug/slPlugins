#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slProcessor.h"

//==============================================================================
class slAudioProcessorEditor : public AudioProcessorEditor
{
public:
    slAudioProcessorEditor (slProcessor&) noexcept;
    slAudioProcessorEditor (slProcessor*) noexcept;
    
protected:
    Rectangle<int> getControlsArea();
    
    void paint (Graphics& g) override;
};