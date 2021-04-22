/*
  ==============================================================================

    MonsterLookAndFeels.h
    Created: 26 Aug 2020 11:00:34am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

static juce::Typeface::Ptr MonsterFontPtr = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf, BinaryData::RobotoThin_ttfSize);
static juce::Font monsterFont = juce::Font(MonsterFontPtr);


class MonsterSliderLAF : public juce::LookAndFeel_V4
{
public:
    float middleAngle = 6.28319f;

    MonsterSliderLAF(){}
    ~MonsterSliderLAF(){}
    
 
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                      float sliderPos,
                      float minSliderPos,
                      float maxSliderPos,
                      const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        
        
        g.setFont(monsterFont);
        
        // DBG(slider.getValue());
        float ypos = (float) y + (float) height * 0.5f;
        float xposRightThumb = x + x + width - sliderPos;
        //auto trackWidth = juce::jmin (6.0f, (float) height * 0.25f);
        float trackWidth = (float)height * 0.13f; // 0.0076
        juce::Point<float> leftThumb(sliderPos, ypos);
        juce::Point<float> rightThumb(xposRightThumb, ypos);
        
        // ================================= Paint Background Track  ================================= //
        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath(x, ypos);
        backgroundTrack.lineTo((float)(width + x), ypos);
        g.setColour(slider.findColour (juce::Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square });
        
        // =================================== Paint Value Track  ====================================  //
        juce::Path valueTrack;
        valueTrack.startNewSubPath(leftThumb);
        valueTrack.lineTo(rightThumb);
        g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.strokePath(valueTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square});
    
        // =================================== Paint Thumbs  ====================================  //
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        g.fillRect(juce::Rectangle<float>(trackWidth, static_cast<float>(height)).withCentre(leftThumb));
        g.fillRect(juce::Rectangle<float>(trackWidth, static_cast<float>(height)).withCentre(rightThumb));
    }
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour(juce::Slider::rotarySliderFillColourId);
        
        auto bounds = juce::Rectangle<float> (x, y, width, height).toFloat().reduced (10);
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float lineW = height * 0.03;
        auto arcRadius = radius - lineW * 0.5f;
        
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, 0.0, 10.0, true);
        g.setColour(outline);
        g.strokePath(backgroundArc, juce::PathStrokeType(height * 0.02, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        if (slider.isEnabled())
        {
            // Paint track from min to max thumb position
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

            g.setColour(fill);
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            //(float strokeThickness, JointStyle joint, EndCapStyle end)
            //                        mitered, curved, beveled :: rounded, square, butt
                                                        

            // Paint from rotary middle position to thumb position
            juce::Path offsetArc;
            auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            offsetArc.addCentredArc (bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, middleAngle, toAngle, true);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(offsetArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        
        // Paint Thumb
        juce::Path thumbPath;
        thumbPath.startNewSubPath(bounds.getCentreX(), bounds.getCentreY());
        thumbPath.lineTo(bounds.getCentreX() + (arcRadius*1.15) * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                         bounds.getCentreY() + (arcRadius*1.15) * std::sin(toAngle - juce::MathConstants<float>::halfPi));
        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.strokePath(thumbPath, juce::PathStrokeType(height * 0.02, juce::PathStrokeType::curved, juce::PathStrokeType::square));
    }
};

class MonsterGainSliderLAF : public juce::LookAndFeel_V4
{
public:
    MonsterGainSliderLAF(){}
    ~MonsterGainSliderLAF(){}
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        //slider.mouseU
        
        float ypos = (float) y + (float) height * 0.5f;
        float xposRightThumb = x + x + width - sliderPos;
        float trackWidth = (float)height * 0.13f; // 0.0076
        
        juce::Point<float> leftThumb(sliderPos, ypos);
        juce::Point<float> rightThumb(xposRightThumb, ypos);
        
        g.setFont(monsterFont);
        // ================================= Paint Background Track  ================================= //
        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath(x, ypos);
        backgroundTrack.lineTo((float)(width + x), ypos);
        g.setColour(slider.findColour (juce::Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square });
        
        // =================================== Paint Value Track  ====================================  //
        juce::Path valueTrack;
        valueTrack.startNewSubPath(x, ypos);
        valueTrack.lineTo(leftThumb);
        g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.strokePath(valueTrack, {trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square});
        
        // =================================== Paint Thumbs  ====================================  //
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        g.fillRect(juce::Rectangle<float>(trackWidth, static_cast<float>(height)).withCentre(leftThumb));
    }
};

class monoButtonLAF : public juce::LookAndFeel_V4
{
private:
    // juce::Font monsterFont = juce::Font(MonsterFontPtr);
    juce::Rectangle<int> allRect, topRect, botRect, topTextRect, botTextRect;
    //juce::Font monsterFont = juce::Font(MonsterFontPtr);
    
    juce::Colour white = juce::Colour(255,255,255);
    juce::Colour black = juce::Colour(10,10,19);
    juce::Colour grey = juce::Colour(100,100,100);
    juce::Colour blue = juce::Colour(89, 204, 240);

    // juce::Colour enabledColour = juce::Colour(3, 223, 252);
    juce::Colour enabledColour = juce::Colour(0,0,0);
    juce::Colour disabledColour = juce::Colour(145, 122, 126);
    juce::Colour hoverColor = juce::Colour(255,0,0);
    juce::Colour pressColor = juce::Colour(0, 255, 0);

    int bw, bh, textStart;
    
public:
    monoButtonLAF(){}
    ~monoButtonLAF(){}
    
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        g.setFont(monsterFont);
        
        bw = button.getWidth(); bh = button.getHeight();
        allRect = juce::Rectangle<int>( 0,  0,    bw, bh   );
        topRect = juce::Rectangle<int>( 0,  0,    bw, bh/2 );
        botRect = juce::Rectangle<int>( 0,  bh/2, bw, bh/2 );
        
        textStart = static_cast<int>(bw * 0.0945);
        topTextRect = topRect.reduced(textStart, 0);
        botTextRect = botRect.reduced(textStart, 0);
        
        // Font (const String &typefaceName, float fontHeight, int styleFlags)
        if (button.getToggleState())
        {
            // DBG("Button mono enabled");
            g.setColour(black); g.fillRect(topRect);
            g.setColour(white); g.fillRect(botRect);
            
            g.setColour(blue); g.drawText(juce::String("Mono"),   topTextRect, juce::Justification::centred);
            g.setColour(black); g.drawText(juce::String("Stereo"), botTextRect, juce::Justification::centred);
            
           // g.setColour(pressColor); g.drawRect(topRect);
            //g.setColour(hoverColor); g.drawRect(botRect);
            
        }
        else
        {
    
            g.setColour(white); g.fillRect(topRect);
            g.setColour(black); g.fillRect(botRect);
            
            g.setColour(black); g.drawText(juce::String("Mono"),   topTextRect, juce::Justification::centred);
            g.setColour(blue); g.drawText(juce::String("Stereo"), botTextRect, juce::Justification::centred);
            
           // g.setColour(hoverColor); g.drawRect(topRect);
            //g.setColour(pressColor); g.drawRect(botRect);
        }
        
        /*
        if (button.getState() == 0) { g.setColour(black); }
        else if (button.getState() == 1) { g.setColour(hoverColor); }
        else { g.setColour(pressColor); }
        */
        
        g.setColour(black); g.drawRect(allRect);
        
    }
};

class toggleButtonLAF : public juce::LookAndFeel_V4
{
private:
    // juce::Font monsterFont = juce::Font(MonsterFontPtr);
    juce::Colour white = juce::Colour(255,255,255);
    juce::Colour black = juce::Colour(10,10,19);
    juce::Colour blue = juce::Colour(89, 204, 240);
    juce::Colour allPathColor;
    
    juce::Colour textColour, buttonColour, outlineColour;
    
    //juce::Font monsterFont = juce::Font(MonsterFontPtr);
    juce::String label;
    
    juce::DropShadow dsDark, dsLight;
    
public:
    toggleButtonLAF()
    {
        //this->setDefaultSansSerifTypeface(MonsterFontPtr);
    }
    ~toggleButtonLAF() {}
    
    void setLabel(juce::String s)
    {
        label = s;
    }
    
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        g.setFont(monsterFont);
        bool use_dropshadow = false;
        // g.setFont(monsterFont);
        int n = 2;
        int blur = 2;
        
        auto bw = button.getWidth(), bh = button.getHeight();
        auto allRect = juce::Rectangle<int>( 0,  0,    bw, bh   );
        
        // Set Colours based on state
        if (button.getState() == 0)
        {
            allPathColor = white;
            
        }
        else if (button.getState() == 1)
        {
            allPathColor = juce::Colour(white);
            
        }
        else
        {
            allPathColor = juce::Colour(white);
        }
        
        
        if (use_dropshadow)
        {
            allRect.reduce(n*2, n*2);
            bw -=n*2; bh -=n*2;
        }
        
        juce::Path allPath;
        allPath.addRoundedRectangle(n, n, bw, bh, n);
        //allPath.addRectangle(allRect);
        
        if (use_dropshadow)
        {

            if (button.getState() == 0)
            {
                dsDark = juce::DropShadow{juce::Colour(100, 100, 100), blur, juce::Point<int>(n,n)};
                dsLight = juce::DropShadow{juce::Colour(255, 255, 255), blur, juce::Point<int>(-n,-n)};
                dsLight.drawForPath(g, allPath);
                dsDark.drawForPath(g, allPath);
                g.setColour(allPathColor);
                g.fillPath(allPath);
            }
            else
            {
                dsDark = juce::DropShadow{juce::Colour(100, 100, 100), blur, juce::Point<int>(0,0)};
                dsLight = juce::DropShadow{juce::Colour(200, 200, 200), blur, juce::Point<int>(n,n)};
                dsLight.drawForPath(g, allPath);
                dsDark.drawForPath(g, allPath);
                g.setColour(allPathColor);
                g.fillRect(allRect.reduced(1, 1));
            }

        }
        else
        {
            g.setColour(allPathColor);
            g.fillPath(allPath);
        }

        int textStartInv = bw * 0.217;
        auto textRect = allRect.reduced(textStartInv, 0);
        
        if (! button.getToggleState())
        {
            textColour = black;
            outlineColour = black;
            buttonColour = white;
        }
        else
        {
            textColour = blue;
            outlineColour = black;
            buttonColour = black;
        }
        
        g.setColour(buttonColour);
        g.fillRect(allRect);
        
        if (button.getButtonText().startsWith("phi"))
        {
            // Draw phase symbol
            juce::Path phaseSymbol;
            float lineW = bh * 0.03;
            auto radius = bh * 0.2f;
            auto arcRadius = radius - lineW * 0.5f;
            
            phaseSymbol.addCentredArc(bw*0.3, bh*0.5, arcRadius, arcRadius, 0.0f, 0.0f, 10.0f, true);
            phaseSymbol.startNewSubPath(bw*0.3, bh*0.75);
            phaseSymbol.lineTo(bw*0.3, bh*0.25);
            
            g.setColour(textColour);
            g.strokePath(phaseSymbol, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.drawText(button.getButtonText().trimCharactersAtStart("phi"), bw*0.15, 0, bw, bh, juce::Justification::centred);
        }
        else
        {
            g.setColour(textColour);
            g.drawText(button.getButtonText(), textRect, juce::Justification::centred);
        }
        
        g.drawRect(allRect);
        
    }
};

