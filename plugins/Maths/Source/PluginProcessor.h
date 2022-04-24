#pragma once

#include <JuceHeader.h>

class MathsAudioProcessorEditor;
//==============================================================================
#define PARAM_P1        "p1"
#define PARAM_P2        "p2"
#define PARAM_P3        "p3"
#define PARAM_P4        "p4"
#define PARAM_LIMITER   "limiter"

class MathsAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    MathsAudioProcessor();
    ~MathsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    void stateUpdated() override;
    void updateState() override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    String lEquation = "(l + r) / 2", rEquation = "(l + r) / 2",
           aEquation = "l", bEquation = "r";
    void setupParsers();
    String lError, rError, aError, bError;

private:
    //==============================================================================
    void setupVars (gin::EquationParser&);
    
    CriticalSection lock;
    LinearSmoothedValue<float> p1Val, p2Val, p3Val, p4Val;
    std::unique_ptr<gin::AudioEquationParser> lParser, rParser, aParser, bParser;
    double l = 0, r = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, t = 0,
           c = 0, s = 0, sr = 0, a = 0, b = 0;
    
    double lo[256] = {0}, ro[256] = {0}, li[256] = {0}, ri[256] = {0}, ao[256] = {0}, bo[256] = {0};
    
    Component::SafePointer<MathsAudioProcessorEditor> editor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MathsAudioProcessor)
};
