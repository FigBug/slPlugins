#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr cycfi::q::frequency low_e          = cycfi::q::pitch_names::E[0];
constexpr cycfi::q::frequency high_e         = cycfi::q::pitch_names::E[7];

//==============================================================================
static gin::ProcessorOptions createProcessorOptions()
{
    return gin::ProcessorOptions()
        .withMidiLearn();
}

PitchTrackAudioProcessor::PitchTrackAudioProcessor()
    : gin::Processor (false, createProcessorOptions())
{
    init();
}

PitchTrackAudioProcessor::~PitchTrackAudioProcessor()
{
}

//==============================================================================
void PitchTrackAudioProcessor::prepareToPlay (double sampleRate_, int blockSize_)
{
    gin::Processor::prepareToPlay (sampleRate_, blockSize_);

    freq = 0.0f;
    
    auto cfg = cycfi::q::signal_conditioner::config();
    conditioner = std::make_unique<cycfi::q::signal_conditioner> (cfg, low_e, high_e, std::uint32_t ( sampleRate_ ) );
    detector = std::make_unique<cycfi::q::pitch_detector> (low_e, high_e, std::uint32_t (sampleRate_), cycfi::q::literals::operator""_dB((long double)-45.0));

    yin = std::make_unique<adamski::PitchYIN> (int (sampleRate_), 512);
    mpm = std::make_unique<adamski::PitchMPM> (int (sampleRate_), 512);

    fifo.setSize (1, std::max (blockSize_ * 2, 512 * 2));
}

void PitchTrackAudioProcessor::releaseResources()
{
}

void PitchTrackAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    if (midiLearn)
        midiLearn->processBlock (midi, buffer.getNumSamples());

    //
    // cycfi::q
    //
    auto& d = *detector;
    auto& c = *conditioner;

    if (buffer.getNumChannels() == 1)
    {
        auto p = buffer.getReadPointer (0);
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            auto v = p[i];
            v = c (v);
            if (d (v))
                freq = detector->get_frequency();
        }
    }
    else
    {
        auto l = buffer.getReadPointer (0);
        auto r = buffer.getReadPointer (1);
        
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            auto v = (l[i] + r[i]) / 2.0f;
            v = c (v);
            if (d (v))
                freq = detector->get_frequency();
        }
    }
    DBG(freq);

    //
    // adamski
    //
    if (buffer.getNumChannels() == 1)
        fifo.write (buffer);
    else
        fifo.write (gin::monoBuffer (buffer));

    while (fifo.getNumReady() >= 512)
    {
        gin::ScratchBuffer buf (1, 512);
        fifo.read (buf);

        auto p1 = yin->getPitchInHz (buf.getReadPointer (0));
        auto p2 = mpm->getPitch (buf.getReadPointer (0));

        DBG(p1);
        DBG(p2);

        freq = (p1 + p2) / 2.0f;
    }
}

float PitchTrackAudioProcessor::getPitch()
{
    return freq;
}

//==============================================================================
bool PitchTrackAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PitchTrackAudioProcessor::createEditor()
{
    return new PitchTrackAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchTrackAudioProcessor();
}
