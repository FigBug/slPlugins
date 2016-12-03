#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class slLookAndFeel : public LookAndFeel_V3,
                      private DeletedAtShutdown
{
public:
    slLookAndFeel();
    
    Colour tyrianPurple;
    Colour prune;
    Colour fireEngineRed;
    Colour veryPaleYellow;
    Colour russianViolet;
    
    juce_DeclareSingleton(slLookAndFeel, false);
};
