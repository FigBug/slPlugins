#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class slLookAndFeel : public LookAndFeel_V3,
                      private DeletedAtShutdown
{
public:
    juce_DeclareSingleton(slLookAndFeel, false);
};