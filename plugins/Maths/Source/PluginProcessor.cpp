#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
MathsAudioProcessor::MathsAudioProcessor()
{
    addExtParam (PARAM_P1,      "p1 (0..1)",  "", "", {0.0f,  1.0f, 0.0f, 1.0f}, 1.0f, 0.0f);
    addExtParam (PARAM_P2,      "p2 (0..1)",  "", "", {0.0f,  1.0f, 0.0f, 1.0f}, 1.0f, 0.0f);
    addExtParam (PARAM_P3,      "p3 (-1..1)", "", "", {-1.0f, 1.0f, 0.0f, 1.0f}, 1.0f, 0.0f);
    addExtParam (PARAM_P4,      "p4 (-1..1)", "", "", {-1.0f, 1.0f, 0.0f, 1.0f}, 1.0f, 0.0f);
    addExtParam (PARAM_LIMITER, "Limiter",    "", "", { 0.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 0.0f, [] (const gin::Parameter&, float v) { return v > 0.5 ? "On" : "Off"; });

    setupParsers();
}

MathsAudioProcessor::~MathsAudioProcessor()
{
}

//==============================================================================
void MathsAudioProcessor::stateUpdated()
{
    lEquation = state.hasProperty ("l") ? state.getProperty ("l") : "(l + r) / 2";
    rEquation = state.hasProperty ("r") ? state.getProperty ("r") : "(l + r) / 2";
    aEquation = state.hasProperty ("a") ? state.getProperty ("a") : "l";
    bEquation = state.hasProperty ("b") ? state.getProperty ("b") : "r";

    setupParsers();
    
    if (editor != nullptr)
        editor->refresh();
}

void MathsAudioProcessor::updateState()
{
    state.setProperty ("l", lEquation, nullptr);
    state.setProperty ("r", rEquation, nullptr);
    state.setProperty ("a", aEquation, nullptr);
    state.setProperty ("b", bEquation, nullptr);
}

//==============================================================================
void MathsAudioProcessor::prepareToPlay (double sampleRate, int)
{
    if (lParser != nullptr) lParser->setSampleRate (sampleRate);
    if (rParser != nullptr) rParser->setSampleRate (sampleRate);
    if (aParser != nullptr) aParser->setSampleRate (sampleRate);
    if (bParser != nullptr) bParser->setSampleRate (sampleRate);

    sr = sampleRate;
    c = -1;
    s = 0;
    t = 0;
    
    p1Val.reset (sampleRate, 0.1);
    p2Val.reset (sampleRate, 0.1);
    p3Val.reset (sampleRate, 0.1);
    p4Val.reset (sampleRate, 0.1);
    
    memset (li, 0, sizeof (li));
    memset (ri, 0, sizeof (ri));
    memset (lo, 0, sizeof (lo));
    memset (ro, 0, sizeof (ro));
    memset (ao, 0, sizeof (ao));
    memset (bo, 0, sizeof (bo));
}

void MathsAudioProcessor::setupParsers()
{
    auto csr = getSampleRate();
    if (csr == 0) csr = 44100.0;
    
    auto setup = [&] (std::unique_ptr<gin::AudioEquationParser>& newP, const juce::String& eq) -> juce::String
    {
        newP->setSampleRate (csr);
        newP->addConstants();
        newP->addUtilities();
        newP->addEffectFilterFunctions();
        newP->setEquation (eq);
        setupVars (*newP);
        newP->evaluate();
        
        if (newP->hasError())
        {
            juce::String err = newP->getError();
            DBG(lError);
            newP = nullptr;
            return err;
        }
        else
        {
            return {};
        }
    };
    
    auto newL = std::make_unique<gin::AudioEquationParser>();
    auto newR = std::make_unique<gin::AudioEquationParser>();
    auto newA = std::make_unique<gin::AudioEquationParser>();
    auto newB = std::make_unique<gin::AudioEquationParser>();

    lError = setup (newL, lEquation);
    rError = setup (newR, rEquation);
    aError = setup (newA, aEquation);
    bError = setup (newB, bEquation);

    {
        juce::ScopedLock sl (lock);

        std::swap (lParser, newL);
        std::swap (rParser, newR);
        std::swap (aParser, newA);
        std::swap (bParser, newB);
    }
}

void MathsAudioProcessor::setupVars (gin::EquationParser& p)
{
    p.addVariable ("l", &l);
    p.addVariable ("r", &r);
    p.addVariable ("p1", &p1);
    p.addVariable ("p2", &p2);
    p.addVariable ("p3", &p3);
    p.addVariable ("p4", &p4);
    p.addVariable ("t", &t);
    p.addVariable ("s", &s);
    p.addVariable ("c", &c);
    p.addVariable ("sr", &sr);
    p.addVariable ("a", &a);
    p.addVariable ("b", &b);

    for (int i = 0; i < 256; i++)
    {
        p.addVariable (juce::String::formatted ("li%d", i), &li[i]);
        p.addVariable (juce::String::formatted ("ri%d", i), &ri[i]);
        p.addVariable (juce::String::formatted ("lo%d", i), &lo[i]);
        p.addVariable (juce::String::formatted ("ro%d", i), &ro[i]);
        p.addVariable (juce::String::formatted ("ao%d", i), &lo[i]);
        p.addVariable (juce::String::formatted ("bo%d", i), &ro[i]);
    }
}

void MathsAudioProcessor::releaseResources()
{
}

void MathsAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    juce::ScopedLock sl (lock);

    juce::ScopedNoDenormals noDenormals;
    
    float* lData = buffer.getWritePointer (0);
    float* rData = buffer.getWritePointer (1);
    
    p1Val.setTargetValue (parameterValue (PARAM_P1));
    p2Val.setTargetValue (parameterValue (PARAM_P2));
    p3Val.setTargetValue (parameterValue (PARAM_P3));
    p4Val.setTargetValue (parameterValue (PARAM_P4));

    if (auto p = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo pos;
        if (p->getCurrentPosition (pos))
        {
            c = pos.timeInSeconds;
        }
    }

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        l = lData[i];
        r = rData[i];
        
        memmove (li + 1, li, (size_t) (juce::numElementsInArray (li) - 1) * sizeof (double));
        memmove (ri + 1, ri, (size_t) (juce::numElementsInArray (ri) - 1) * sizeof (double));
        memmove (lo + 1, lo, (size_t) (juce::numElementsInArray (lo) - 1) * sizeof (double));
        memmove (ro + 1, ro, (size_t) (juce::numElementsInArray (ro) - 1) * sizeof (double));
        memmove (ao + 1, ao, (size_t) (juce::numElementsInArray (ao) - 1) * sizeof (double));
        memmove (bo + 1, bo, (size_t) (juce::numElementsInArray (bo) - 1) * sizeof (double));

        li[0] = l;
        ri[0] = r;
        lo[0] = 0;
        ro[0] = 0;

        p1 = p1Val.getNextValue();
        p2 = p2Val.getNextValue();
        p3 = p3Val.getNextValue();
        p4 = p4Val.getNextValue();

        a = (aParser != nullptr) ? aParser->evaluate() : 0.0;
        if (std::isnan (a) || std::isinf (a)) a = 0.0f;
        ao[0] = a;
        
        b = (bParser != nullptr) ? bParser->evaluate() : 0.0;
        if (std::isnan (b) || std::isinf (b)) b = 0.0f;
        bo[0] = b;

        double l2 = (lParser != nullptr) ? lParser->evaluate() : 0.0;
        if (std::isnan (l2) || std::isinf (l2)) l2 = 0.0f;
        lo[0] = l2;

        double r2 = (rParser != nullptr) ? rParser->evaluate() : 0.0;
        if (std::isnan (r2) || std::isinf (r2)) r2 = 0.0f;
        ro[0] = r2;
        
        if (parameterIntValue (PARAM_LIMITER) != 0)
        {
            l2 = jlimit (-1.0, 1.0, l2);
            r2 = jlimit (-1.0, 1.0, r2);
        }
        
        lData[i] = float (l2);
        rData[i] = float (r2);
        
        if (c != -1) c = 1 / sr;
        t += 1 / sr;
        s++;
    }
}

//==============================================================================
bool MathsAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MathsAudioProcessor::createEditor()
{
    editor = new MathsAudioProcessorEditor (*this);
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MathsAudioProcessor();
}
