/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Measurer.h"
#include "Utilities.h"

static const juce::Identifier treeID = "Tree";
static const juce::Identifier typeID = "Type";
static const juce::Identifier idID = "ID";

//==============================================================================
/**
*/
class Processor  : public AudioProcessor, private PropertyChangeListener
{
public:
    //==============================================================================
    Processor();
    ~Processor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // shared pointer
    Measurer::ptr measurer_;
    
    enum class Type {Off, Start, End};
    
    ValueTree tree_ {treeID};
    CachedValue <Type> typeField_ {tree_, typeID, nullptr, Type::Off};
    CachedValue <int> idField_ {tree_, idID, nullptr, 0};

private:
    std::atomic <Type> type_ { Type::Off };
    std::atomic <int> id_ { 0 }; // between 0 and maxNumChannelsToMeasure_ - 1
    
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

template<>
struct VariantConverter <Processor::Type>
{
    static Processor::Type fromVar (const juce::var& v)
    {
        const auto s = v.toString();
        if (s == "Start")
            return Processor::Type::Start;
        if (s == "End")
            return Processor::Type::End;
        return Processor::Type::Off;
    }
    
    static juce::var toVar (const Processor::Type& c)
    {
        if (c == Processor::Type::Start)
            return "Start";
        if (c == Processor::Type::End)
            return "End";
        return "Off";
    }
};
