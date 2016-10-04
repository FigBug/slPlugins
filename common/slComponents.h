#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slParameter.h"

//==============================================================================
class PluginSlider : public Slider,
                     public Slider::Listener,
                     public ChangeListener
{
public:
    PluginSlider (slParameter* parameter_, SliderStyle style, TextEntryBoxPosition textBoxPosition) : Slider (style, textBoxPosition), parameter (parameter_)
    {
        addListener (this);
        setRange (parameter->getUserRangeStart(), parameter->getUseRangeEnd());
        setValue (parameter->getUserValue(), juce::dontSendNotification);
        
        parameter->addChangeListener (this);
    }
    
    ~PluginSlider()
    {
        parameter->removeChangeListener (this);
    }
    
    void sliderValueChanged (Slider*) override
    {
        parameter->setUserValueNotifingHost (float (getValue()));
        setValue (parameter->getUserValue());
    }
    
    void sliderDragStarted (Slider*) override
    {
        parameter->beginUserAction();
    }
    
    void sliderDragEnded (Slider*) override
    {
        parameter->endUserAction();
    }
    
    void changeListenerCallback (ChangeBroadcaster*) override
    {
        setValue (parameter->getUserValue(), dontSendNotification);
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        if (e.mods.isAltDown())
            parameter->setUserValue (parameter->getUserDefaultValue());
        else if (e.mods.isLeftButtonDown())
            Slider::mouseDown (e);
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown())
            Slider::mouseUp (e);
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if (e.mods.isLeftButtonDown())
            Slider::mouseDrag (e);
    }
    
private:
    slParameter* parameter;
};

//==============================================================================
class Readout : public Label,
                private ChangeListener
{
public:
    Readout (slParameter* parameter);
    ~Readout();
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    slParameter* parameter;
};

//==============================================================================
class ParamComponent : public Component
{
public:
    ParamComponent (slParameter* parameter);
};

//==============================================================================
class Knob : public ParamComponent
{
public:
    Knob (slParameter* parameter);
    
private:
    void resized() override;
    
    Label name;
    Readout value;
    PluginSlider knob;
};