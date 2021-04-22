/*
  ==============================================================================

    Background.h
    Created: 19 Feb 2021 10:12:25am
    Author:  admin

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>

class Background  : public juce::Component
{

public:
    Background() {}
  
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(40,40,40));
    }
    
private: JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Background)
};


