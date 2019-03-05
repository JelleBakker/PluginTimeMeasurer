/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ProcessorEditor  : public AudioProcessorEditor, private Timer, private PropertyChangeListener
{
public:
    ProcessorEditor (Processor&);
    ~ProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    Processor& processor_;
    
private:
    
    Image image_;
    juce::Rectangle <int> imageBounds_;
    
    TextButton btnOff_ {"Off"}, btnStart_ {"Start"}, btnEnd_ {"End"};
    Label labelType_ {"", "Position/type:"};
    ComboBox comboID_;
    Label labelID_ {"", "ID:"};
    
    void timerCallback() override;
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorEditor)
};
