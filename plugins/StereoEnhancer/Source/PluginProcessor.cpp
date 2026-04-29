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
                  .getChildFile ("Library/Audio/Presets/SocaLabs/StereoEnhancer");
       #elif JUCE_WINDOWS
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("SocaLabs/StereoEnhancer");
       #else
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("SocaLabs/StereoEnhancer");
       #endif
    }

    juce::File systemResourceRoot()
    {
       #if JUCE_MAC
        return juce::File ("/Library/Audio/Presets/SocaLabs/StereoEnhancer");
       #elif JUCE_WINDOWS
        return juce::File::getSpecialLocation (juce::File::commonApplicationDataDirectory)
                  .getChildFile ("SocaLabs/StereoEnhancer");
       #else
        return juce::File ("/usr/share/SocaLabs/StereoEnhancer");
       #endif
    }

    juce::File legacyUserProgramDirectory()
    {
       #if JUCE_MAC
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("Application Support/com.socalabs/StereoEnhancer/programs");
       #else
        return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                  .getChildFile ("com.socalabs/StereoEnhancer/programs");
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
    widthLP     = addExtParam ("widthLP",       "Width",    "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.5f, 0.1f, percentTextFunction);
    freqHPFader = addExtParam ("freqHPFader",   "Freq HP",  "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.5f, 0.1f, percentTextFunction);
    widthHP     = addExtParam ("widthHP",       "Width HP", "", "%",   { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f, 0.1f, percentTextFunction);
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

            proc.setParameters (widthLP->getProcValue (1), freqHPFader->getProcValue (1), widthHP->getProcValue (1));

            proc.process (workBuffer);
            pos++;
        }
    }
    else
    {
        proc.setParameters (widthLP->getProcValue (numSamples), freqHPFader->getProcValue (numSamples), widthHP->getProcValue (numSamples));

        proc.process (buffer);
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

