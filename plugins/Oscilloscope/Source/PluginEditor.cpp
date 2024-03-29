#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), scopeProc (p)
{
    addAndMakeVisible (&scope);

    auto c1 = juce::Colours::white.overlaidWith (juce::Colours::blue.withAlpha (0.3f));
    auto c2 = juce::Colours::white.overlaidWith (juce::Colours::yellow.withAlpha (0.3f));

    scope.setColour (gin::TriggeredScope::lineColourId, findColour (gin::PluginLookAndFeel::grey45ColourId));

    scope.setColour (gin::TriggeredScope::traceColourId + 0, c1);
    scope.setColour (gin::TriggeredScope::traceColourId + 1, c2);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 0, c1);
    scope.setColour (gin::TriggeredScope::envelopeColourId + 1, c2);

    for (auto pp : p.getPluginParameters())
    {
        gin::ParamComponent* pc;

        if (pp->getUid() == PARAM_TRIGGER_MODE || pp->getUid() == PARAM_TRIGGER_CHANNEL || pp->getUid() == PARAM_TRIGGER_RUN)
            pc = new gin::Select (pp);
        else if (pp->isOnOff())
            pc = new gin::Switch (pp);
        else
            pc = new gin::Knob (pp);

        addAndMakeVisible (pc);
        controls.add (pc);
    }

    setGridSize (8, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);

    scope.setNumChannels (p.getTotalNumInputChannels());

    for (auto pp : scopeProc.getPluginParameters())
        pp->addListener (this);

    updateScope();
}

PluginEditor::~PluginEditor()
{
    for (auto pp : scopeProc.getPluginParameters())
        pp->removeListener (this);
}

//==============================================================================

juce::Rectangle<int> PluginEditor::getGridArea (int x, int y, int w, int h)
{
    return juce::Rectangle<int> (getWidth() - inset - 3 * cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    scope.setBounds (inset, headerHeight + inset, getWidth() - 3 * cx - inset - 10, getHeight() - headerHeight - 2 * inset);

    componentForId (PARAM_SAMPLES_PER_PIXEL)->setBounds (getGridArea (0, 0));
    componentForId (PARAM_VERTICAL_ZOOM)->setBounds (getGridArea (0, 1));
    componentForId (PARAM_VERTICAL_OFFSET_L)->setBounds (getGridArea (0, 2));
    componentForId (PARAM_VERTICAL_OFFSET_R)->setBounds (getGridArea (0, 3));

    componentForId (PARAM_TRIGGER_CHANNEL)->setBounds (getGridArea (1, 0));
    componentForId (PARAM_TRIGGER_MODE)->setBounds (getGridArea (2, 0));
    componentForId (PARAM_TRIGGER_RUN)->setBounds (getGridArea (1, 1));
    componentForId (PARAM_TRIGGER_RESET)->setBounds (getGridArea (2, 1));

    componentForId (PARAM_TRIGGER_LEVEL)->setBounds (getGridArea (1, 3));
    componentForId (PARAM_TRIGGER_POS)->setBounds (getGridArea (2, 3));
}

void PluginEditor::updateScope()
{
    scope.setDrawTriggerPos (true);
    scope.setNumSamplesPerPixel (scopeProc.parameterIntValue (PARAM_SAMPLES_PER_PIXEL));
    scope.setSingleTrigger (scopeProc.parameterBoolValue (PARAM_TRIGGER_RUN));
    scope.setVerticalZoomFactor (scopeProc.parameterValue (PARAM_VERTICAL_ZOOM));
    scope.setVerticalZoomOffset (scopeProc.parameterValue (PARAM_VERTICAL_OFFSET_L), 0);
    scope.setVerticalZoomOffset (scopeProc.parameterValue (PARAM_VERTICAL_OFFSET_R), 1);
    scope.setTriggerChannel (scopeProc.parameterIntValue (PARAM_TRIGGER_CHANNEL));
    scope.setTriggerMode ((gin::TriggeredScope::TriggerMode)scopeProc.parameterIntValue (PARAM_TRIGGER_MODE));
    scope.setTriggerLevel (scopeProc.parameterValue (PARAM_TRIGGER_LEVEL));
    scope.setTriggerPos (scopeProc.parameterValue (PARAM_TRIGGER_POS));

    if (scopeProc.parameterBoolValue (PARAM_TRIGGER_RESET))
    {
        scope.resetTrigger();
        juce::Timer::callAfterDelay (50, [this] { scopeProc.getParameter (PARAM_TRIGGER_RESET)->setValueNotifyingHost (0.0f); });
    }
}
