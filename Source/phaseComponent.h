/*
  ==============================================================================

    phaseComponent.h
    Created: 24 Feb 2021 9:18:56am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "JollLookAndFeel.h"
#include "DesignHelper.h"

class PhaseComponent: public juce::Component
{
public:
    juce::Rectangle<int> leftArea;
    juce::Rectangle<int> rightArea;

    PhaseComponent(){}

    void resized() override
    {
        leftArea = designHelper.subRectangle(getLocalBounds(), 16, 26, 100, 28);
        rightArea = designHelper.subRectangle(getLocalBounds(), 252, 26, 100, 28);
    }


    void paint (juce::Graphics& g) override
    {
        g.setFont(designHelper.robotoRegularFont);
        g.setColour(juce::Colour(170, 170, 170));
        g.drawText("L", leftArea, juce::Justification::right, true);
        g.drawText("R", rightArea, juce::Justification::left, true);
    }
    
private: JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseComponent)
};


