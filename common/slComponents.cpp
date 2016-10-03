/*
  ==============================================================================

    slComponents.cpp
    Created: 3 Nov 2015 9:33:48am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "slComponents.h"

//==============================================================================
Readout::Readout (slParameter* parameter_)
  : parameter (parameter_)
{
    parameter->addChangeListener (this);
    setText (parameter->getUserValueText(), dontSendNotification);
}

Readout::~Readout()
{
    parameter->removeChangeListener (this);
}

void Readout::changeListenerCallback (ChangeBroadcaster*)
{
    setText (parameter->getUserValueText(), dontSendNotification);
}

//==============================================================================
Knob::Knob (slParameter* parameter)
  : name ("", ""),
    value (parameter),
    knob (parameter, Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox)
{
    addAndMakeVisible (name);
    addAndMakeVisible (value);
    addAndMakeVisible (knob);
 
    knob.setSkewFactor (parameter->getSkew());
    
    name.setText (parameter->getName (1000), dontSendNotification);
    value.setJustificationType (Justification::centred);
    name.setJustificationType (Justification::centred);
}

void Knob::resized()
{
    Rectangle<int> r = getLocalBounds();
    
    name.setBounds (r.removeFromTop (20));
    value.setBounds (r.removeFromBottom (20));
    knob.setBounds (r);
}