#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class XYScope : public Component,
                public Timer,
                public TimeSliceClient
{
public:

    XYScope (TimeSliceThread* backgroundThreadToUse = nullptr);

    ~XYScope();
    
    enum ColourIds
    {
        lineColourId             = 0x1231e10,
        backgroundColourId       = 0x1231e11, 
        traceColourId            = 0x1231e12
    };
    
    void addSamples (const AudioSampleBuffer& buffer);

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    int useTimeSlice() override;

private:
    //==============================================================================
    OptionalScopedPointer<TimeSliceThread> backgroundThreadToUse;
    
    struct Channel
    {
        Channel() :
          numLeftToAverage (4),
          bufferSize (4096),
          bufferWritePos (0),
          minBuffer ((size_t) bufferSize),
          maxBuffer ((size_t) bufferSize),
          currentMax (-1.0f),
          currentMin (1.0f),
          samplesToProcess (32768),
          tempProcessingBlock (32768)
        {}
        
        int numLeftToAverage;
        int bufferSize, bufferWritePos;

        HeapBlock<float> minBuffer, maxBuffer;

        float currentMax, currentMin;
        drow::FifoBuffer<float> samplesToProcess;
        HeapBlock<float> tempProcessingBlock;
    };
    
    OwnedArray<Channel> channels;
    
    bool needToUpdate;

    bool needToRepaint;
    Image image;
    CriticalSection imageLock;

    //==============================================================================
    void processPendingSamples();
    void renderImage();
    int getTriggerPos();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYScope)
};

