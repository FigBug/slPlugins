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
    void paint (Graphics& g) override;
};