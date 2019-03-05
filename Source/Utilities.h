/*
  ==============================================================================

    Utilities.h
    Created: 5 Mar 2019 10:48:55am
    Author:  Jelle

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct PropertyChangeListener : ValueTree::Listener
{
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override {}
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) override {}
};
