#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
#define DEG_CIRCLE 360
#define DEG_TO_RAD (double_Pi / (DEG_CIRCLE / 2))
#define RAD_TO_DEG ((DEG_CIRCLE / 2) / double_Pi)

//==============================================================================
inline double deg2rad(double degrees)
{
    return degrees * DEG_TO_RAD;
}

inline double rad2deg(double radians)
{
    return radians * RAD_TO_DEG;
}

//==============================================================================
template <typename T>
inline bool almostEqual (T a, T b, T precision = 0.00001)
{
    return std::abs (a - b) < precision;
}

//==============================================================================
class LevelTracker
{
public:
    LevelTracker (float decayPerSecond);
    
    void trackBuffer (const float* buffer, int numSamples);
    void trackBuffer (AudioSampleBuffer& buffer);
    
    float getLevel();
    bool getClip()      { return clip;  }
    void clearClip()    { clip = false; }
    
protected:
    float peakTime  {0.0f};
    float peakLevel {-100.0f};
    float decayRate {0.0f};
    bool clip {false};
};

//==============================================================================
class LevelMeter : public Component,
                   private Timer
{
public:
    LevelMeter (LevelTracker& tracker_) : tracker (tracker_)
    {
        setColour (backgroundColourId, Colours::darkgrey);
        setColour (meterColourId, Colours::yellow);
        
        startTimerHz (30);
    }
    
    void setLevel (float level_)
    {
        if (std::fabs(level - level_))
        {
            level = level_;
            repaint();
        }
    }
    
    enum ColourIds
    {
        backgroundColourId              = 0x6500000,
        meterColourId                   = 0x6500001
    };
    
private:
    void paint (Graphics& g) override
    {
        Rectangle<int> r = getLocalBounds();
        
        g.setColour (findColour (backgroundColourId));
        g.fillRect (r);
        
        float pos = range.convertTo0to1(jlimit(range.start, range.end, level));
        
        r = r.removeFromBottom (roundFloatToInt (pos * getHeight()));
        
        g.setColour (findColour (meterColourId));
        g.fillRect (r);
    }
    
    void timerCallback() override
    {
        setLevel (tracker.getLevel());
    }
    
    float level {0};
    NormalisableRange<float> range {-48.0, 0};
    LevelTracker& tracker;
};

//==============================================================================
template<typename Type>
inline bool updateIfDifferent (Type& valueToUpdate, const Type& newValue) noexcept
{
    if (valueToUpdate == newValue)
        return false;
    
    valueToUpdate = newValue;
    return true;
}
