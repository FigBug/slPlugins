#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

static juce::String percentTextFunction (const gin::Parameter&, float v)
{
    return juce::String (juce::roundToInt (v * 100));
}

//==============================================================================
//==============================================================================
namespace
{
    juce::File userResourceRoot()
    {
       #if JUCE_MAC
        return juce::File::getSpecialLocation (juce::File::userHomeDirectory)
                  .getChildFile ("Library/Audio/Presets/SocaLabs/SimpleVerb");
       #elif JUCE_WINDOWS
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("SocaLabs/SimpleVerb");
       #else
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("SocaLabs/SimpleVerb");
       #endif
    }

    juce::File systemResourceRoot()
    {
       #if JUCE_MAC
        return juce::File ("/Library/Audio/Presets/SocaLabs/SimpleVerb");
       #elif JUCE_WINDOWS
        return juce::File::getSpecialLocation (juce::File::commonApplicationDataDirectory)
                  .getChildFile ("SocaLabs/SimpleVerb");
       #else
        return juce::File ("/usr/share/SocaLabs/SimpleVerb");
       #endif
    }

    juce::File legacyUserProgramDirectory()
    {
       #if JUCE_MAC
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("Application Support/com.socalabs/SimpleVerb/programs");
       #else
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("com.socalabs/SimpleVerb/programs");
       #endif
    }
}

static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withAdditionalCredits ({"Michael \"LOSER\" Gruhn"})
        .withMidiLearn();
}

AudioProcessor::AudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    room        = addExtParam ("room",      "Room",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.0f, percentTextFunction);
    damp        = addExtParam ("damp",      "Damp",     "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.6f, 0.1f, percentTextFunction);
    preDelay    = addExtParam ("predelay",  "PreDelay", "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.0f, percentTextFunction);
    lp          = addExtParam ("lp",        "LP",       "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.9f, 0.1f, percentTextFunction);
    hp          = addExtParam ("hp",        "HP",       "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.1f, 0.1f, percentTextFunction);
    wet         = addExtParam ("wet",       "Wet",      "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.4f, 0.1f, percentTextFunction);
    dry         = addExtParam ("dry",       "Dry",      "", "%",   {   0.0f,   1.0f, 0.0f, 1.0f}, 0.3f, 0.1f, percentTextFunction);
    // One-time migration of any user presets from the pre-installer location.
    // Factory presets now live in systemResourceRoot()/Presets and are surfaced
    // via getFactoryProgramDirectories(). User saves go to userResourceRoot()/Presets.
    {
        auto oldDir = legacyUserProgramDirectory();
        auto newDir = userResourceRoot().getChildFile ("Presets");
        if (oldDir.isDirectory()
            && newDir.findChildFiles (juce::File::findFiles, false, "*.xml").isEmpty())
        {
            if (! newDir.isDirectory())
                newDir.createDirectory();
            for (auto f : oldDir.findChildFiles (juce::File::findFiles, false, "*.xml"))
                f.copyFileTo (newDir.getChildFile (f.getFileName()));
        }
    }
    init();
}

AudioProcessor::~AudioProcessor()
{
}

//==============================================================================
void AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
    
    reverb.setSampleRate (float (sampleRate));
}

void AudioProcessor::reset()
{
    gin::Processor::reset();
}

void AudioProcessor::releaseResources()
{
}

void AudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
    if (isSmoothing())
    {
        int pos = 0;
        
        while (pos < numSamples)
        {
            auto workBuffer = gin::sliceBuffer (buffer, pos, 1);
            
            reverb.setParameters (room->getProcValue (1), damp->getProcValue (1),
                                  preDelay->getProcValue (1), lp->getProcValue (1),
                                  hp->getProcValue (1), wet->getProcValue (1),
                                  dry->getProcValue (1));

            reverb.process (workBuffer);
            pos++;
        }
    }
    else
    {
        reverb.setParameters (room->getProcValue (numSamples), damp->getProcValue (numSamples),
                              preDelay->getProcValue (numSamples), lp->getProcValue (numSamples),
                              hp->getProcValue (numSamples), wet->getProcValue (numSamples),
                              dry->getProcValue (numSamples));
        
        reverb.process (buffer);
    }
}

//==============================================================================
bool AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioProcessor::createEditor()
{
    return new AudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessor();
}
//==============================================================================
juce::File AudioProcessor::getProgramDirectory()
{
    auto dir = userResourceRoot().getChildFile ("Presets");
    if (! dir.isDirectory())
        dir.createDirectory();
    return dir;
}

juce::Array<juce::File> AudioProcessor::getFactoryProgramDirectories()
{
    return { systemResourceRoot().getChildFile ("Presets") };
}

