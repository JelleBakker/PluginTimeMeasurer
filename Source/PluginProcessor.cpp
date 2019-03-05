/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Processor::Processor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    tree_.addListener (this);
    typeField_.setValue (typeField_.getDefault(), nullptr);
    idField_.setValue (idField_.getDefault(), nullptr);
}

Processor::~Processor() { }

//==============================================================================
const String Processor::getName() const { return JucePlugin_Name; }
bool Processor::acceptsMidi() const { return false; }
bool Processor::producesMidi() const { return false; }
bool Processor::isMidiEffect() const { return false; }
double Processor::getTailLengthSeconds() const { return 0.0; }
int Processor::getNumPrograms() { return 1; }
int Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram (int index) {}
const String Processor::getProgramName (int index) { return {}; }
void Processor::changeProgramName (int index, const String& newName) {}

//==============================================================================
void Processor::prepareToPlay (double sampleRate, int samplesPerBlock) {}
void Processor::releaseResources(){}
bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const { return true; }

void Processor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    const auto t = type_.load();
    const auto ID = juce::jlimit (0, Measurer::maxNumChannelsToMeasure_ - 1, id_.load());
    if (t == Type::Start)
        measurer_->setStart (ID, juce::Time::getHighResolutionTicks());
    else if (t == Type::End)
        measurer_->setEnd (ID, juce::Time::getHighResolutionTicks());
}

//==============================================================================
bool Processor::hasEditor() const { return true; }
AudioProcessorEditor* Processor::createEditor() { return new ProcessorEditor (*this); }

//==============================================================================
void Processor::getStateInformation (MemoryBlock& destData)
{
    ValueTree tree (treeID);
    tree.setProperty (typeID, typeField_.getPropertyAsValue().toString(), nullptr);
    tree.setProperty (idID, idField_.get(), nullptr);
    
    std::unique_ptr<XmlElement> xml (tree.createXml());
    copyXmlToBinary (*xml, destData);
}

void Processor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        const auto tree = ValueTree::fromXml (*xmlState);
        if (tree.isValid())
        {
            tree_.setProperty (typeField_.getPropertyID(), tree.getProperty (typeField_.getPropertyID()), nullptr);
            tree_.setProperty (idField_.getPropertyID(), tree.getProperty (idField_.getPropertyID()), nullptr);
        }
    }
}

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }

void Processor::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged == tree_)
    {
        if (property == typeField_.getPropertyID())
        {
            typeField_.forceUpdateOfCachedValue();
            type_.store (typeField_.get());
        }
        else if (property == idField_.getPropertyID())
        {
            idField_.forceUpdateOfCachedValue();
            id_.store (idField_.get());
        }
        else jassertfalse;
    }
    else jassertfalse;
}
