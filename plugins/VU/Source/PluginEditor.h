#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VUMeter.h"

//==============================================================================
/**
*/
class PluginEditor  : public gin::ProcessorEditor,
                      private juce::ValueTree::Listener
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void resized() override;
    void addMenuItems (juce::PopupMenu&) override;

private:
    void addReferenceItems (juce::PopupMenu&);
    void showReferenceMenu();
    void updateCaptions();
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override   { updateCaptions(); }
    void valueTreeRedirected (juce::ValueTree&) override                                 { updateCaptions(); }

    int meterWidthForWidth (int w) const;
    int heightForWidth (int w) const;

    static constexpr int meterGap = 8;

    PluginProcessor& proc;

    VUMeter meterL { proc.level[0], proc.peak[0], "L" };
    VUMeter meterR { proc.level[1], proc.peak[1], "R" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
