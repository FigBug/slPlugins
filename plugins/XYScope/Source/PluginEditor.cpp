#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : gin::ProcessorEditor (p), proc (p)
{
    addAndMakeVisible (&scope);
    
    for (auto pp : p.getPluginParameters())
    {
        gin::ParamComponent* pc;

        if (pp->getUid() == PARAM_MODE)
            pc = new gin::Select (pp);
        else if (pp->isOnOff())
            pc = new gin::Switch (pp);
        else
            pc = new gin::Knob (pp);

        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (7, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
    
    for (auto pp : proc.getPluginParameters())
        pp->addListener (this);

    updateScope();
}

PluginEditor::~PluginEditor()
{
    for (auto pp : proc.getPluginParameters())
        pp->removeListener (this);
}

//==============================================================================

Rectangle<int> PluginEditor::getGridArea (int x, int y, int w, int h)
{
    return Rectangle<int> (getWidth() - inset - cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    gin::ProcessorEditor::resized();

    auto rc = Rectangle<int> (inset, headerHeight + inset, getWidth() - cx - 2 * inset, getHeight() - headerHeight - 2 * inset);
    
    scope.setBounds (rc);
    
    componentForId (PARAM_MODE)->setBounds (getGridArea (0, 0));
    componentForId (PARAM_LOG)->setBounds (getGridArea (0, 1));
}

void PluginEditor::updateScope()
{
}
