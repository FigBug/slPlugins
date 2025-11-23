#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioProcessorEditor : public gin::ProcessorEditor
{
public:
    AudioProcessorEditor (AudioProcessor&);
    ~AudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    AudioProcessor& proc;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorEditor)
};
