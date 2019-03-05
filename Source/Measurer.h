/*
  ==============================================================================

    Measurer.h
    Created: 4 Mar 2019 12:46:25pm
    Author:  Jelle

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

class ProcessorEditor;

class Measurer : private AsyncUpdater
{
private:
    // Measurer can only be instantiated by the friend class Measurer
    Measurer() {}
public:
    ~Measurer() {}
    
    static constexpr int maxNumChannelsToMeasure_ = 16;
    
    struct Result { int64 startToStart; int64 startToEnd; };
    
    typedef SharedResourcePointer <Measurer> ptr;
    friend class SharedResourcePointer <Measurer>;
    
    //==============================================================================
    void editorOpened (ProcessorEditor&);
    void editorClosed (ProcessorEditor&);
    void updateAfterEditorsOpenedList();
    
    void setStart (int channelID, int64 start);
    void setEnd (int channelID, int64 end);
    
    // returns true if something was read
    bool readFifo (int channelID, Result& result); // called from "after" gui
    
private:
    std::array <int64, maxNumChannelsToMeasure_> lastStartArray_ {};
    std::array <int64, maxNumChannelsToMeasure_> lastEndArray_ {};
    
    static constexpr int fifoSize = 44100;
    typedef std::array <Result, fifoSize> ChannelFifo;
    std::array <ChannelFifo, maxNumChannelsToMeasure_> channelFifoBufferArray_;
    std::array <AbstractFifo, maxNumChannelsToMeasure_> abstractFifoArray_
    {{  {fifoSize}, {fifoSize}, {fifoSize}, {fifoSize},
        {fifoSize}, {fifoSize}, {fifoSize}, {fifoSize},
        {fifoSize}, {fifoSize}, {fifoSize}, {fifoSize},
        {fifoSize}, {fifoSize}, {fifoSize}, {fifoSize},
    }};
    
    std::array <bool, maxNumChannelsToMeasure_> endGUIIsOpenedArray_ {};
    
    std::vector <std::reference_wrapper <ProcessorEditor>> openedEditors_;
    
    void handleAsyncUpdate() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Measurer)
};
