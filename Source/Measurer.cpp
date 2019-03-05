/*
  ==============================================================================

    Measurer.cpp
    Created: 4 Mar 2019 12:51:47pm
    Author:  Jelle

  ==============================================================================
*/

#include "Measurer.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

void Measurer::editorOpened (ProcessorEditor& newEditor)
{
    for (auto& e : openedEditors_)
    {
        if (&e.get() == &newEditor)
        {
            jassertfalse;
            return;
        }
    }
    openedEditors_.push_back (newEditor);
    updateAfterEditorsOpenedList();
}

void Measurer::editorClosed (ProcessorEditor& editor)
{
    for (auto it = openedEditors_.begin(); it != openedEditors_.end(); it++)
    {
        if (&it->get() == &editor)
        {
            openedEditors_.erase(it);
            updateAfterEditorsOpenedList();
            return;
        }
    }
    jassertfalse;
}

void Measurer::updateAfterEditorsOpenedList()
{
    triggerAsyncUpdate();
}

void Measurer::setStart (int channelID, int64 start)
{
    jassert (channelID >= 0 && channelID < maxNumChannelsToMeasure_);
    if (endGUIIsOpenedArray_ [channelID])
    {
        auto& fifo = abstractFifoArray_ [channelID];
        int start1, size1, start2, size2;
        fifo.prepareToWrite(1, start1, size1, start2, size2);
        if (size1 > 0 && lastStartArray_ [channelID] != 0 && lastEndArray_ [channelID] != 0)
        {
            channelFifoBufferArray_ [channelID][start1] = { start - lastStartArray_ [channelID],
                                                            lastEndArray_ [channelID] - lastStartArray_ [channelID] };
            fifo.finishedWrite (1);
        }
        else fifo.finishedWrite (0);
    }
    lastStartArray_ [channelID] = start;
}

void Measurer::setEnd (int channelID, int64 end)
{
    jassert (channelID >= 0 && channelID < maxNumChannelsToMeasure_);
    lastEndArray_ [channelID] = end;
}

bool Measurer::readFifo (int channelID, Result& result)
{
    jassert (channelID >= 0 && channelID < maxNumChannelsToMeasure_);
    auto& fifo = abstractFifoArray_ [channelID];
    int start1, size1, start2, size2;
    fifo.prepareToRead (1, start1, size1, start2, size2);
    if (size1 > 0)
        result = channelFifoBufferArray_[channelID][start1];
    fifo.finishedRead (size1);
    return size1 > 0;
}

void Measurer::handleAsyncUpdate()
{
    auto isEditorWithEndTypeOpened = [&] (int ID)
    {
        for (auto& e : openedEditors_)
        {
            auto& proc = e.get().processor_;
            proc.typeField_.forceUpdateOfCachedValue();
            proc.idField_.forceUpdateOfCachedValue();
            if (proc.idField_ == ID && proc.typeField_ == Processor::Type::End)
                return true;
        }
        return false;
    };
    for (int i = 0; i < maxNumChannelsToMeasure_; i++)
        endGUIIsOpenedArray_[i] = isEditorWithEndTypeOpened (i);
}
