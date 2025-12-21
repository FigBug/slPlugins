#pragma once

#include <JuceHeader.h>

class AudioRecorder : public juce::Thread
{
public:
    enum class RecordMode
    {
        idle,
        normal,
        triggered,
        waitingForTrigger
    };

    AudioRecorder (gin::AudioFifo& f)
        : juce::Thread ("AudioRecorder"), fifo (f)
    {
        startThread();
    }

    ~AudioRecorder() override
    {
        stopRecording();
        stopThread (1000);
    }

    void setSampleRate (double sr)
    {
        sampleRate = sr;
        retrospectiveBuffer.setSize (fifo.getNumChannels(), int (sr * 30));
    }

    void setTriggerLevel (float level)
    {
        triggerLevel = level;
    }

    void setDestFolder (const juce::File& folder)
    {
        destFolder = folder;
    }

    juce::File getDestFolder() const
    {
        return destFolder;
    }

    void startRecording()
    {
        if (recordMode.load() != RecordMode::idle)
            return;

        createNewFile();
        recordMode.store (RecordMode::normal);
    }

    void startTriggeredRecording()
    {
        if (recordMode.load() != RecordMode::idle)
            return;

        recordMode.store (RecordMode::waitingForTrigger);
    }

    void stopRecording()
    {
        recordMode.store (RecordMode::idle);

        const juce::ScopedLock sl (writerLock);
        if (writer != nullptr)
        {
            writer->flush();
            writer.reset();
        }
    }

    bool isRecording() const
    {
        auto mode = recordMode.load();
        return mode == RecordMode::normal || mode == RecordMode::triggered;
    }

    bool isWaitingForTrigger() const
    {
        return recordMode.load() == RecordMode::waitingForTrigger;
    }

    RecordMode getRecordMode() const
    {
        return recordMode.load();
    }

    void saveRetrospective()
    {
        const juce::SpinLock::ScopedLockType sl (retroLock);

        auto numReady = retrospectiveBuffer.getNumReady();
        if (numReady == 0)
            return;

        createNewFile();

        gin::ScratchBuffer buffer (retrospectiveBuffer.getNumChannels(), numReady);
        retrospectiveBuffer.peek (buffer);

        const juce::ScopedLock wl (writerLock);
        if (writer != nullptr)
        {
            writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
            writer->flush();
            writer.reset();
        }
    }

    std::function<void()> onRecordingStarted;
    std::function<void()> onRecordingStopped;

    void run() override
    {
        while (! threadShouldExit())
        {
            auto ready = fifo.getNumReady();
            if (ready > 0)
            {
                gin::ScratchBuffer buffer (fifo.getNumChannels(), ready);
                fifo.read (buffer);

                // Always write to retrospective buffer
                {
                    const juce::SpinLock::ScopedLockType sl (retroLock);
                    retrospectiveBuffer.ensureFreeSpace (ready);
                    retrospectiveBuffer.write (buffer);
                }

                // Handle recording
                auto mode = recordMode.load();

                if (mode == RecordMode::waitingForTrigger)
                {
                    // Check if audio level exceeds trigger threshold
                    float maxLevel = 0.0f;
                    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                        maxLevel = std::max (maxLevel, buffer.getMagnitude (ch, 0, buffer.getNumSamples()));

                    if (maxLevel >= std::abs (triggerLevel.load()))
                    {
                        createNewFile();

                        // Write pre-roll from retrospective buffer (0.1 seconds before trigger)
                        {
                            const juce::SpinLock::ScopedLockType sl (retroLock);

                            const int availableSamples = retrospectiveBuffer.getNumReady();

                            if (availableSamples > 0)
                            {
                                // Peek entire buffer
                                gin::ScratchBuffer fullBuffer (retrospectiveBuffer.getNumChannels(), availableSamples);
                                retrospectiveBuffer.peek (fullBuffer);

                                // Only write the last 0.1 seconds
                                const int preRollSamples = std::min (int (sampleRate * 0.1), availableSamples);
                                const int startSample = availableSamples - preRollSamples;

                                const juce::ScopedLock wl (writerLock);
                                if (writer != nullptr)
                                    writer->writeFromAudioSampleBuffer (fullBuffer, startSample, preRollSamples);
                            }
                        }

                        recordMode.store (RecordMode::triggered);
                        mode = RecordMode::triggered;

                        if (onRecordingStarted)
                            juce::MessageManager::callAsync (onRecordingStarted);
                    }
                }

                if (mode == RecordMode::normal || mode == RecordMode::triggered)
                {
                    const juce::ScopedLock sl (writerLock);
                    if (writer != nullptr)
                        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
                }
            }

            if (fifo.getNumReady() == 0)
                juce::Thread::sleep (10);
        }
    }

private:
    void createNewFile()
    {
        auto time = juce::Time::getCurrentTime();
        auto filename = time.formatted ("%Y-%m-%d_%H-%M-%S") + ".wav";
        auto file = destFolder.getChildFile (filename);

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::OutputStream> stream (file.createOutputStream());

        if (stream != nullptr)
        {
            auto options = juce::AudioFormatWriterOptions()
                .withSampleRate (sampleRate)
                .withChannelLayout (juce::AudioChannelSet::canonicalChannelSet (fifo.getNumChannels()))
                .withBitsPerSample (16);

            const juce::ScopedLock sl (writerLock);
            writer = wavFormat.createWriterFor (stream, options);
        }
    }

    gin::AudioFifo& fifo;
    gin::AudioFifo retrospectiveBuffer;
    juce::SpinLock retroLock;
    juce::File destFolder { juce::File::getSpecialLocation (juce::File::userDesktopDirectory) };

    std::atomic<RecordMode> recordMode { RecordMode::idle };
    std::atomic<float> triggerLevel { 0.0f };
    double sampleRate = 44100.0;
    juce::CriticalSection writerLock;
    std::unique_ptr<juce::AudioFormatWriter> writer;
};
