#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "slParameter.h"

class slProcessor;

class slProgram
{
public:
    void loadProcessor (slProcessor* p);
    void saveProcessor (slProcessor* p);
    
    void loadFromFile (File f);
    void saveToDir (File f);
    void deleteFromDir (File f);
    
    String name;
    Array<slParameter::ParamState> states;
};
