/*
  ==============================================================================

    Voice.cpp
    Created: 4 Jul 2019 8:25:21pm
    Author:  Roland Rabien

  ==============================================================================
*/

#include "Voice.h"
#include "PluginProcessor.h"

void Voice::noteStarted()
{
    if (auto p = synth.getPadForNote (currentlyPlayingNote.initialNote))
    {
        sfxr.reset (true);
        sfxr.setParams (p->params);
    }
    else
    {
        clearCurrentNote();
    }
}

void Voice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    float work[numSamples];
    memset (work, 0, sizeof (work));
    
    if (sfxr.synthWave (work, 0, numSamples))
        clearCurrentNote();
    
    for (int ch = 0; ch < outputBuffer.getNumSamples(); ch++)
        FloatVectorOperations::add (outputBuffer.getWritePointer (ch, startSample), work, numSamples);
}
