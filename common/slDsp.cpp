#include "slDsp.h"

//==============================================================================
void applyGain (AudioSampleBuffer& buffer, LinearSmoothedValue<float>& gain)
{
    if (gain.isSmoothing())
    {
        if (float** w = buffer.getArrayOfWritePointers())
        {
            for (int s = 0; s < buffer.getNumSamples(); s++)
            {
                float g = gain.getNextValue();
                for (int c = 0; c < buffer.getNumChannels(); c++)
                    w[c][s] *= g;
            }
        }

    }
    else
    {
        buffer.applyGain (gain.getTargetValue());
    }
}
