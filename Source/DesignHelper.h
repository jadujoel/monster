/*
  ==============================================================================

    DesignHelper.h
    Created: 19 Feb 2021 5:16:44pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DesignHelper
{
private:
    float scale = 1;
    float widthScale = 1;
    float heightScale = 1;

    float designFontSize = 14;
    
    float designWidth;
    float designHeight;
    
    float actualWidth;
    float actualHeight;

public:
    juce::Typeface::Ptr robotoThinFontPtr = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf, BinaryData::RobotoThin_ttfSize);
    juce::Typeface::Ptr robotoRegularFontPtr = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf, BinaryData::RobotoRegular_ttfSize);
    juce::Font robotoThinFont = juce::Font(robotoThinFontPtr);
    juce::Font robotoRegularFont = juce::Font(robotoRegularFontPtr);

    DesignHelper() {}
    
    void setDesignWidth(float width)
    {
        designWidth = width;
    }
    
    void setDesignHeight(float height)
    {
        designHeight = height;
    }
    
    void setActualWidth(float width)
    {
        actualWidth = width;
        widthScale = actualWidth / designWidth;
        updateScale();
    }
    
    void setActualHeight(float height)
    {
        actualHeight = height;
        heightScale = actualHeight / designHeight;
        updateScale();
    }

    void setDesignSize(float width, float height, float fontSize)
    {
        designWidth = width;
        designHeight = height;
        designFontSize = fontSize;
    }

    void setSize(float width, float height)
    {
        actualWidth = width;
        actualHeight = height;
        widthScale = actualWidth / designWidth;
        heightScale = actualHeight / designHeight;
        updateScale();
    }
    
    void setBounds(juce::Rectangle<int> area)
    {
        setSize(static_cast<float>(area.getWidth()), static_cast<float>(area.getHeight()));
    }

    void rescale(float _scale)
    {
        scale = _scale;
        robotoRegularFont.setHeight(designFontSize * scale);
        robotoThinFont.setHeight(designFontSize * scale);
        //// thebug("robotoFontHeight", robotoRegularFont.getHeight());
        setSize(scaled(designWidth), scaled(designHeight));
    }
    

    juce::Rectangle<int> scaledRectangle(int x, int y, int width, int height)
    {
        return juce::Rectangle<int> (
            static_cast<int>(std::rint(x * widthScale)),
            static_cast<int>(std::rint(y * heightScale)),
            static_cast<int>(std::rint(width * widthScale)),
            static_cast<int>(std::rint(height * heightScale)));
    }

    float scaled (float val)
    {
        return val * scale;
    }

    int scaled (int val)
    {
        return static_cast<int>(std::rint((float) val * scale));
    }

    void updateScale()
    {
        scale = (widthScale + heightScale) / 2;
    }

    int atleastOne(int val)
    {
        return juce::jmax(1, val);
    }


    float atleastOne(float val)
    {
        return juce::jmax(1.0f, val);
    }

    juce::Rectangle<int> subRectangle(juce::Rectangle<int> parentRectangle, int x, int y, int width, int height)
    {
        return juce::Rectangle<int> (
            static_cast<int>(parentRectangle.getX() + std::rint(x * widthScale)),
            static_cast<int>(parentRectangle.getY() + std::rint(y * heightScale)),
            static_cast<int>(atleastOne(rint(width * widthScale))),
            static_cast<int>(atleastOne(static_cast<int>(std::rint(height * heightScale)))));
    }

    juce::Rectangle<float> subRectangle(juce::Rectangle<float> parentRectangle, float x, float y, float width, float height)
    {
        return juce::Rectangle<float> (
                parentRectangle.getX() + x * widthScale,
                parentRectangle.getY() + y * heightScale,
                width * widthScale,
                height * heightScale);
    }

    juce::Rectangle<float> subRectangle(const juce::Rectangle<int>& parentRectangle, float x, float y, float width, float height)
    {
        return juce::Rectangle<float> (
                parentRectangle.getX() + x * widthScale,
                parentRectangle.getY() + y * heightScale,
                width * widthScale,
                height * heightScale);
    }

    template <class T>
    juce::Point<T> getCentreTop(juce::Rectangle<T> rect)
    {
        return juce::Point<T> (rect.getCentreX(), rect.getY());
    }


    template <class T>
    juce::Rectangle<T> smaller(juce::Rectangle<T>& rect, float reduction, bool doScale = true)
    {
        if (doScale) { reduction = scaled(reduction); };
        return juce::Rectangle<T> (rect.getX() + reduction, rect.getY() + reduction,
                rect.getWidth() - 2 * reduction, rect.getHeight() - 2 * reduction);
    }

    template <class T>
    juce::Rectangle<T> ellipseBounds(juce::Rectangle<T> area, float strokeThickness)
    {
        return smaller(area, strokeThickness / 2.0f);
    }

    juce::Rectangle<float> subEllipseBounds(juce::Rectangle<float> area, float x, float y, float width, float height, float strokeThickness)
    {
        return ellipseBounds(subRectangle(area, x, y, width, height), strokeThickness);
    }

    template <class T>
    juce::Point<T> getCentreBottom(juce::Rectangle<T> rect)
    {
        return juce::Point<T> (rect.getCentreX(), rect.getBottom());
    }

    juce::Rectangle<float> rectangleFloatFromInt (juce::Rectangle<int> rect)
    {
        return juce::Rectangle<float> (static_cast<float>(rect.getX()), static_cast<float>(rect.getY()), static_cast<float>(rect.getWidth()), static_cast<float>(rect.getHeight()));
    }

    juce::Rectangle<int> rectangleIntFromFloat (juce::Rectangle<float> rect)
    {
        return juce::Rectangle<int> (static_cast<int>(rect.getX()), static_cast<int>(rect.getY()), static_cast<int>(rect.getWidth()), static_cast<int>(rect.getHeight()));
    }



    void drawCircle(juce::Graphics &g, juce::Rectangle<float> bounds, float lineThickness)
    {
        g.drawRoundedRectangle(bounds, 9999.0f, lineThickness);
    }

    void drawCircle(juce::Graphics &g, juce::Rectangle<int> bounds, float lineThickness)
    {
        g.drawRoundedRectangle(rectangleFloatFromInt(bounds), 9999.0f, lineThickness);
    }

    void setGradientVertical(juce::Graphics& g,
            const juce::Rectangle<int>& area,
            const juce::Colour& topColour,
            const juce::Colour& botColour)
    {
        // (Colour colour1, float x1, float y1,
        //  Colour colour2, float x2, float y2, bool radial)
        g.setGradientFill(juce::ColourGradient (
                topColour, 0, (float) area.getY(),
                botColour, 0, (float) area.getBottom(),
                false));

    }

    void drawVerticalGradient (
            juce::Graphics& g,
            const juce::Rectangle<int>& area,
            const juce::Colour& baseColour = juce::Colours::grey)
    {
        g.setGradientFill (juce::ColourGradient (
                baseColour.brighter (0.5f), 0, (float) area.getY(),
                baseColour.darker (0.5f), 0, (float) area.getBottom(),
                false));

        g.fillRect(area);
    }
} static designHelper;


