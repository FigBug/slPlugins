#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class slParameter : public AudioProcessorParameter,
                    public ChangeBroadcaster,
                    public Timer
{
public:
    slParameter (String uid, String name, String label, float minValue, float maxValue, float intervalValue, float defaultValue, float skewFactor = 1.0f);
    
    String getUid() { return uid; }

    float getUserValue() const;
    float getUserDefaultValue() const;
    void setUserValue(float v);
    void setUserValueNotifingHost(float f);
    String getUserValueText() const;
    
    void beginUserAction();
    void beginUserTimedAction();
    void endUserAction();
    void timerCallback() override;

    NormalisableRange<float> getUserRange() { return range;         }
    float getUserRangeStart()               { return range.start;   }
    float getUseRangeEnd()                  { return range.end;     }
    
    struct ParamState
    {
        String uid;
        float value;
    };
    
    ParamState getState();
    void setState(const ParamState& state);
    
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    
    String getName (int maximumStringLength) const override;
    String getLabel() const override;
    
    int getNumSteps() const override;
    String getText (float value, int /*maximumStringLength*/) const override;
    float getValueForText (const String& text) const override;
    
    bool isOrientationInverted() const override;
    bool isAutomatable() const override;
    bool isMetaParameter() const override;
    
    float getSkew() { return skewFactor; }
    
protected:
    NormalisableRange<float> range;
    
    float value;
    float defaultValue;
    float skewFactor;
    
    String uid;
    String name;
    String label;
    
    int userActionCount {0};
};

