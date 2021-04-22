/*
  ==============================================================================

    Container.h
    Created: 19 Feb 2021 10:10:15am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Container  : public juce::Component
{
private:
    int shadowOffsetX = 0;
    int shadowOffsetY = 4;
    int radius = 4;
    float cornerSize = 4;

    juce::Colour shadowColor = juce::Colours::black.withAlpha(0.5f);
    juce::Colour fillColor = juce::Colour(72,72,74);
    
    juce::Point<int> offset;
    juce::Rectangle<int> shadowBounds = getBounds();
    juce::Rectangle<int> fillBounds = getBounds();
    
    juce::Path path;
    
public:
    Container()
    {
        setPaintingIsUnclipped(true);
    }
    
    void resized() override
    {
        cornerSize = designHelper.scaled(4.0f);
        radius = shadowOffsetY = static_cast<int>(cornerSize);
        offset.setXY(0, shadowOffsetY);
        
        path = juce::Path();
        path.addRoundedRectangle(getLocalBounds(), cornerSize);
    }
    
    void paint (juce::Graphics& g) override
    {
        // Draw a shadow
        path.setUsingNonZeroWinding(false);
        juce::DropShadow dropShadow(shadowColor, radius, offset);
        dropShadow.drawForPath(g, path);
        
        // Draw the fill color
        g.setColour(fillColor);
        g.fillPath(path);
    }
    
private: JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Container)
};
