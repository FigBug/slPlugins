#include "slParameter.h"
#include "slUtil.h"

slParameter::slParameter (String uid_, String name_, String label_, float minValue, float maxValue, float intervalValue, float defaultValue_, float skewFactor_)
  : value (defaultValue_),
    defaultValue (defaultValue_),
    skewFactor (skewFactor_),
    uid (uid_),
    name (name_),
    label (label_)
{
    range = NormalisableRange<float> (minValue, maxValue, intervalValue, skewFactor);
}

float slParameter::getUserValue() const
{
    return jlimit (range.start, range.end, value);
}

float slParameter::getUserDefaultValue() const
{
    return defaultValue;
}

void slParameter::setUserValue (float v)
{
    v = jlimit(range.start, range.end, range.snapToLegalValue (v));
    if (! almostEqual (value, v))
    {
        value = v;
        
        triggerAsyncUpdate();
    }
}

void slParameter::setUserValueNotifingHost (float v)
{
    v = jlimit(range.start, range.end, range.snapToLegalValue (v));
    if (! almostEqual (value, v))
    {
        value = v;
        setValueNotifyingHost (getValue());
        
        triggerAsyncUpdate();
    }
}

void slParameter::setUserValueAsUserAction (float f)
{
    beginUserAction();
    setUserValueNotifingHost (f);
    endUserAction();
}

String slParameter::getUserValueText() const
{
    return getText (getValue(), 1000);
}

void slParameter::beginUserAction()
{
    userActionCount++;
    if (userActionCount == 1)
        beginChangeGesture();
}

void slParameter::endUserAction()
{
    userActionCount--;
    if (userActionCount == 0)
        endChangeGesture();
}

void slParameter::beginUserTimedAction()
{
    if (! isTimerRunning())
        beginUserAction();
    
    startTimer (2000);
}

void slParameter::timerCallback()
{
    endUserAction();
    stopTimer();
}

void slParameter::addListener (Listener* listener)
{
    listeners.add (listener);
}

void slParameter::removeListener (Listener* listener)
{
    listeners.remove (listener);
}

void slParameter::handleAsyncUpdate()
{
    listeners.call (&Listener::parameterChanged, this);
}

slParameter::ParamState slParameter::getState()
{
    ParamState state;
    state.uid = uid;
    state.value = getUserValue();
    return state;
}

void slParameter::setState (const ParamState& state)
{
    jassert (state.uid == uid);
    setUserValue (state.value);
}

float slParameter::getValue() const
{
    return jlimit (0.0f, 1.0f, range.convertTo0to1 (value));
}

void slParameter::setValue (float valueIn)
{
    valueIn = jlimit (0.0f, 1.0f, valueIn);
    float newValue = range.snapToLegalValue (range.convertFrom0to1 (valueIn));
    
    if (! almostEqual(value, newValue))
    {
        value = newValue;
        triggerAsyncUpdate();
    }
}

float slParameter::getDefaultValue() const
{
    return range.convertTo0to1 (defaultValue);
}

String slParameter::getName (int maximumStringLength) const
{
    return name.substring (0, maximumStringLength);
}

String slParameter::getLabel() const
{
    return label;
}

int slParameter::getNumSteps() const
{
    if (range.interval == 0)
        return 0;
    return roundFloatToInt ((range.end - range.start) / range.interval);
}

String slParameter::getText (float val, int /*maximumStringLength*/) const
{
    return String (range.snapToLegalValue (range.convertFrom0to1 (val)), 1) + " " + label;
}

float slParameter::getValueForText (const String& text) const
{
    return range.convertTo0to1 (text.getFloatValue());
}

bool slParameter::isOrientationInverted() const
{
    return false;
}

bool slParameter::isAutomatable() const
{
    return true;
}

bool slParameter::isMetaParameter() const
{
    return false;
}


