/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProcessorEditor::ProcessorEditor (Processor& p)
    : AudioProcessorEditor (&p), processor_ (p)
{
    startTimerHz (30);
    
    //==============================================================================
    addAndMakeVisible (btnOff_);
    addAndMakeVisible (btnStart_);
    addAndMakeVisible (btnEnd_);
    btnOff_.setRadioGroupId (1);
    btnStart_.setRadioGroupId (1);
    btnEnd_.setRadioGroupId (1);
    btnOff_.setConnectedEdges (Button::ConnectedOnRight);
    btnStart_.setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnEnd_.setConnectedEdges (Button::ConnectedOnLeft);
    btnOff_.setClickingTogglesState (true);
    btnStart_.setClickingTogglesState (true);
    btnEnd_.setClickingTogglesState (true);
    btnOff_.onClick = btnStart_.onClick = btnEnd_.onClick = [&]
    {
        if (btnOff_.getToggleState())
            processor_.typeField_ = Processor::Type::Off;
        else if (btnStart_.getToggleState())
            processor_.typeField_ = Processor::Type::Start;
        else processor_.typeField_ = Processor::Type::End;
    };
    labelType_.attachToComponent (&btnOff_, true);
    
    //==============================================================================
    addAndMakeVisible (comboID_);
    for (int i = 0; i < Measurer::maxNumChannelsToMeasure_; i++)
        comboID_.addItem (juce::String (i + 1), i + 1);
    comboID_.onChange = [&] { processor_.idField_ = comboID_.getSelectedItemIndex(); };
    addAndMakeVisible (labelID_);
    labelID_.attachToComponent (&comboID_, true);
    
    processor_.tree_.addListener (this);
    valueTreePropertyChanged (processor_.tree_, processor_.typeField_.getPropertyID());
    valueTreePropertyChanged (processor_.tree_, processor_.idField_.getPropertyID());
    
    setSize (400, 300);
    
    processor_.measurer_->editorOpened (*this);
}

ProcessorEditor::~ProcessorEditor()
{
    processor_.measurer_->editorClosed (*this);
    processor_.tree_.removeListener (this);
}

//==============================================================================
void ProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    if (processor_.typeField_ == Processor::Type::End)
        g.drawImageAt (image_, imageBounds_.getX(), imageBounds_.getY());
}

void ProcessorEditor::resized()
{
    btnOff_.setBounds (100, 20, 50, 30);
    btnStart_.setBounds (150, 20, 50, 30);
    btnEnd_.setBounds (200, 20, 50, 30);
    comboID_.setBounds (100, 55, 80, 30);
    
    imageBounds_ = {10, 140, getWidth() - 20, getHeight() - 140 - 10};
    image_ = juce::Image (juce::Image::RGB, imageBounds_.getWidth(), imageBounds_.getHeight(), true);
    image_.clear (image_.getBounds());
}

void ProcessorEditor::timerCallback()
{
    if (processor_.typeField_ == Processor::Type::End)
    {
        bool anythingRead = false;
        Measurer::Result result;
        while (processor_.measurer_->readFifo (processor_.idField_, result))
        {
            image_.moveImageSection (0, 0, 1, 0, image_.getWidth() - 1, image_.getHeight());
            image_.clear ({image_.getWidth() - 1, 0, 1, image_.getHeight()}); // clear last line
            juce::Graphics g (image_);
            
            auto cpu = (float)result.startToEnd / (float)result.startToStart;
            cpu = juce::jlimit (0.0f, 1.0f, cpu);
            
            auto startY = (1.0f - cpu) * image_.getHeight();
            g.setColour (juce::Colours::green);
            g.fillRect (image_.getWidth() - 1.0f, startY, 1.0f, image_.getHeight() - startY);
            anythingRead = true;
        }
        if (anythingRead)
            repaint();
    }
}

void ProcessorEditor::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged == processor_.tree_)
    {
        if (property == processor_.typeField_.getPropertyID())
        {
            processor_.typeField_.forceUpdateOfCachedValue();
            if (processor_.typeField_ == Processor::Type::Off)
                btnOff_.setToggleState (true, dontSendNotification);
            else if (processor_.typeField_ == Processor::Type::Start)
                btnStart_.setToggleState (true, dontSendNotification);
            else btnEnd_.setToggleState (true, dontSendNotification);
        }
        else if (property == processor_.idField_.getPropertyID())
        {
            processor_.idField_.forceUpdateOfCachedValue();
            comboID_.setSelectedItemIndex (processor_.idField_, juce::dontSendNotification);
        }
        else jassertfalse;
        processor_.measurer_->updateAfterEditorsOpenedList();
        image_.clear (image_.getBounds());
        repaint();
    }
    else jassertfalse;
}
