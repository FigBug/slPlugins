#include "slUtil.h"

LevelTracker::LevelTracker (float decayPerSecond) : decayRate (decayPerSecond)
{
}

void LevelTracker::trackBuffer (const float* buffer, int numSamples)
{
    Range<float> range = FloatVectorOperations::findMinAndMax (buffer, numSamples);
    float v1 = std::fabs (range.getStart());
    float v2 = std::fabs (range.getEnd());
    
    float peakDB = Decibels::gainToDecibels (jmax (v1, v2));
    
    const float time = (Time::getMillisecondCounter() / 1000.0f);
    
    if (getLevel() < peakDB)
    {
        peakLevel = peakDB;
        peakTime = time;
    }
}

float LevelTracker::getLevel()
{
    const float hold = 50.0f / 1000.0f;
    
    const float elapsed = (Time::getMillisecondCounter() / 1000.0f) - peakTime;
    
    if (elapsed < hold)
        return peakLevel;
    return peakLevel - (decayRate * (elapsed - hold));
}

