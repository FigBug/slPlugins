#include "slProgram.h"
#include "slProcessor.h"

void slProgram::loadProcessor (slProcessor* p)
{
    for (slParameter::ParamState state : states)
    {
        if (slParameter* pp = p->getParameter (state.uid))
            if (!pp->isMetaParameter())
                pp->setUserValueNotifingHost (state.value);
    }
}

void slProgram::saveProcessor (slProcessor* p)
{
    states.clear();
    
    Array<slParameter*> params = p->getPluginParameters();
    for (slParameter* param : params)
        if (! param->isMetaParameter())
            states.add (param->getState());
}

void slProgram::loadFromFile (File f)
{
    XmlDocument doc (f);
    ScopedPointer<XmlElement> rootE (doc.getDocumentElement());
    if (rootE)
    {
        states.clear();
        
        name = rootE->getStringAttribute("name");
        
        XmlElement* paramE = rootE->getChildByName("param");
        while (paramE)
        {
            String uid = paramE->getStringAttribute("uid");
            float  val = (float) paramE->getDoubleAttribute("val");

            slParameter::ParamState state;
            state.uid   = uid;
            state.value = val;
            states.add (state);
            
            paramE = paramE->getNextElementWithTagName("param");
        }
    }
}

void slProgram::saveToDir (File f)
{
    ScopedPointer<XmlElement> rootE (new XmlElement ("state"));
    
    rootE->setAttribute("name", name);
    
    for (slParameter::ParamState state : states)
    {
        XmlElement* paramE = new XmlElement ("param");
        
        paramE->setAttribute("uid", state.uid);
        paramE->setAttribute("val", state.value);
        
        rootE->addChildElement (paramE);
    }
    
    File xmlFile = f.getChildFile(File::createLegalFileName(name) + ".xml");
    rootE->writeToFile(xmlFile, String::empty);
}

void slProgram::deleteFromDir (File f)
{
    f.getChildFile (File::createLegalFileName(name) + ".xml").deleteFile();
}

