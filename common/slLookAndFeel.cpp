#include "slLookAndFeel.h"

juce_ImplementSingleton(slLookAndFeel);

//==============================================================================
slLookAndFeel::slLookAndFeel()
: tyrianPurple (0xff561643),
  prune (0xff6c0e23),
  fireEngineRed (0xffc42021),
  veryPaleYellow (0xfff3ffb9),
  russianViolet (0xff3c1742)
{
    setColour (Label::textColourId, russianViolet);
}
