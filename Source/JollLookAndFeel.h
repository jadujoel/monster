/*
  ==============================================================================

    jollLookAndFeel.h
    Created: 9 Feb 2021 12:05:31pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "JollLabel.h"
#include "DesignHelper.h"
#include "Debug.h"

const bool WOODEN_PC = true;


class JollLookAndFeel : public juce::LookAndFeel_V4
{

private:
    float rotaryTrackWidth;
    float rotaryThumbWidth;
    int rotaryTrackShadowRadius;
    juce::Point<int> rotaryTrackShadowOffset;

    // Linear Slider Constants / Variables
    const juce::Colour linearSliderBackgColor = juce::Colour(72, 72, 74);
    const juce::Colour linearSliderTrackColor = juce::Colour(44, 44, 46);
    const juce::Colour linearSliderHandleColor = juce::Colour(72, 72, 74);
    const juce::Colour linearSliderLabelColor = juce::Colour(170, 170, 170);
    const juce::Colour linearSliderValueColor = juce::Colour(255, 255, 255);

public:
    // Scaled float
    float sf (float val) { return designHelper.scaled(val); }
    float smf (float val) { return designHelper.atleastOne(designHelper.scaled(val)); }
    // Scaled Int
    int si (int val) { return designHelper.scaled(val); }
    int si (float val) { return static_cast<int>(designHelper.scaled(val)); }
    // Scaled int with min value of 1;
    int smi (int val) {
        return designHelper.atleastOne(designHelper.scaled(val));
    }
    void resize()
    {
        rotaryTrackWidth = sf(14.0f);
        rotaryThumbWidth = sf(2.0f);
        rotaryTrackShadowRadius = smi(2);
        rotaryTrackShadowOffset.setXY(0, smi(2));
    }


public:
    
    JollLookAndFeel(){}
    ~JollLookAndFeel(){}

    //    void fillTextEditorBackground (juce::Graphics&, int width, int height, juce::TextEditor&) override;
    //    void drawTextEditorOutline (juce::Graphics&, int width, int height, juce::TextEditor&) override;
    //    void drawPropertyComponentBackground (juce::Graphics&, int width, int height, juce::PropertyComponent&) override;
    //    void drawPropertyComponentLabel (juce::Graphics&, int width, int height, juce::PropertyComponent&) override;

  /*  void paintToolbarBackground	(juce::Graphics &g, int width, int height,
            juce::Toolbar & toolbar) override
    {
        // thebug("paintToolbarBackground called");
    }*/

    //void drawConcertinaPanelHeader	(juce::Graphics &g, const juce::Rectangle<int> &area, bool isMouseOver, bool isMouseDown,
    //        juce::ConcertinaPanel &concertinaPanel,
    //        juce::Component &panel
    //) override
    //{
    //    // thebug("DrawConcertinaPanelHeader Called");
    //}

    //void drawMenuBarBackground	(juce::Graphics &g,
    //        int 	width,
    //        int 	height,
    //        bool 	isMouseOverBar,
    //        juce::MenuBarComponent &menuBarComponent
    //) override
    //{
    //    // thebug("drawMenuBarBackground Called");
    //}

    void drawPhaseButton(juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        juce::ignoreUnused(shouldDrawButtonAsHighlighted);
        juce::ignoreUnused(shouldDrawButtonAsDown);
        auto buttonBounds = designHelper.rectangleFloatFromInt(button.getLocalBounds());
        auto outerBounds = designHelper.subRectangle(buttonBounds, 8, 8, 44, 44);
        auto innerBounds = designHelper.subRectangle(outerBounds, 4.0f, 4.0f, 36.0f, 36.0f);
        auto innerBoundsMinusGradient = designHelper.smaller(innerBounds, 1.0f);
        //auto innerBoundsMinusGradient = designHelper.subRectangle(innerBounds, 1.0f, 1.0f, 34.0f, 34.0f);

        float iconStrokeThickness = 2;
        // auto iconBounds = designHelper.subRectangle(innerBounds, 10.0f, 10.0f, 16.0f, 20.0f);
        auto iconEllipseBounds = designHelper.subEllipseBounds(buttonBounds, 22.0f, 22.0f, 16.0f, 16.0f, iconStrokeThickness);
        auto iconRectangleBounds = designHelper.subRectangle(buttonBounds, 30.0f, 20.0f, 1.0f, 20.0f);

        juce::Colour transparent = juce::Colours::white.withAlpha(0.0f);
        juce::Colour grey(44,44,46);
        juce::Colour greyLight(72, 72, 76);
        juce::Colour toggleColor = greyLight;
        juce::Colour shadowColor = juce::Colours::black.withAlpha(0.5f);
        juce::ColourGradient gradient(juce::Colours::white.withAlpha(0.2f), designHelper.getCentreTop(innerBounds),
                juce::Colours::black.withAlpha(0.2f), designHelper.getCentreBottom(innerBounds), false);

        if (button.getToggleState())
        {
            toggleColor = juce::Colours::white;
            shadowColor = transparent;
            gradient.setColour(0, juce::Colours::white);
            gradient.setColour(1, juce::Colours::white);
        }

        juce::Path frame;
        frame.addRoundedRectangle(outerBounds, sf(8));
        g.setColour(grey);
        g.fillPath(frame);
        drawInnerShadow(g, frame, juce::Colours::black.withAlpha(0.25f), si(2), {0, si(2)}); // changed

        juce::Path toggle;
        toggle.addRoundedRectangle(innerBounds, sf(4));
        juce::DropShadow shadow(shadowColor, si(4), {0, smi(4)});
        shadow.drawForPath(g, toggle);
        g.setColour(greyLight);
        g.fillPath(toggle);

        g.setGradientFill(gradient);
        g.fillPath(toggle);

        g.setColour(toggleColor);
        g.fillRoundedRectangle(innerBoundsMinusGradient, sf(4));

        juce::Path icon;
        icon.addEllipse(iconEllipseBounds);
        icon.addLineSegment(juce::Line(iconRectangleBounds.getTopLeft(), iconRectangleBounds.getBottomLeft()), 0.0f);
        g.setColour(grey);
        g.strokePath(icon, juce::PathStrokeType(static_cast<float>(smi(2))));
    }

    void drawMonoButton (juce::Graphics &g, juce::ToggleButton &button)
    {
        auto buttonBounds = designHelper.rectangleFloatFromInt(button.getLocalBounds());
        auto mainBounds = designHelper.subRectangle(buttonBounds, 122.0f, 18.0f, 140.0f, 100.0f);
        auto toggleBounds = designHelper.subRectangle(mainBounds, 8.0f, 12.0f, 124.0f, 76.0f);
        auto trackBounds = designHelper.subRectangle(mainBounds, 8, 12, 124, 76);
        auto gradientBounds = designHelper.subRectangle(toggleBounds, 4.0f, 4.0f, 68.0f, 68.0f);
        if (!button.getToggleState()) { gradientBounds.setX(trackBounds.getRight() - gradientBounds.getWidth() - sf(4)); }
        auto circleBounds = designHelper.smaller(gradientBounds, 1.0f);

        auto leftTextBounds = designHelper.subRectangle(buttonBounds, 53.66f, 60.0f, 57.44f, 16.0f);
        auto rightTextBounds = designHelper.subRectangle(buttonBounds, 271.95f, 60.0f, 57.44f, 16.0f);

        juce::Colour leftTextColour;
        juce::Colour rightTextColour;

        if (!button.getToggleState())
        {
            leftTextColour = juce::Colours::black;
            rightTextColour = juce::Colours::white;
        } else {
            leftTextColour = juce::Colours::white;
            rightTextColour = juce::Colours::black;
        }

        g.setFont(designHelper.robotoRegularFont);
        g.setColour(leftTextColour);
        g.drawText("Mono", leftTextBounds, juce::Justification::right, true);
        g.setColour(rightTextColour);
        g.drawText("Stereo", rightTextBounds, juce::Justification::left, true);

        float cornerSize = sf(38);
        // // thebug("CornerSize", cornerSize);

        g.setColour(juce::Colour(44, 44, 46));
        juce::Path track;
        //track.addRoundedRectangle(trackBounds, sf(sf(sf(sf(38.0f)))));
        track.addRoundedRectangle(trackBounds, cornerSize);
        //track.addRoundedRectangle(<#Rectangle<ValueType> rectangle#>, <#float cornerSize#>)
        g.fillPath(track);
        drawInnerShadow(g, track, juce::Colours::black.withAlpha(0.25f), si(2), {0, si(2)});

        juce::ColourGradient gradient(juce::Colours::white.withAlpha(0.3f), designHelper.getCentreTop(gradientBounds),
            juce::Colours::black.withAlpha(0.3f), designHelper.getCentreBottom(gradientBounds), false);

        juce::Path protrusion;
        protrusion.addRoundedRectangle(gradientBounds, 999.0f);
        juce::DropShadow dropShadowProtrusion(juce::Colours::black.withAlpha(0.5f), si(8), {0, si(8)});
        dropShadowProtrusion.drawForPath(g, protrusion);
        g.setGradientFill(gradient);
        g.fillPath(protrusion);

        g.setColour(juce::Colour(72, 72, 74));
        g.fillRoundedRectangle(circleBounds, 999.0f);
    }

    void drawToggleButton (juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        if (button.getComponentID() == juce::String("phase"))
        {
            drawPhaseButton (g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        }
        else if (button.getComponentID() == juce::String("mono"))
        {
            drawMonoButton(g, button);
        }
    }

    juce::Slider::SliderLayout getSliderLayout (juce::Slider &slider) override
    {
        juce::Slider::SliderLayout layout;
        
        int x = slider.getX();
        int y = slider.getY();
        int w = slider.getWidth();
        int h = slider.getHeight();
        
        juce::ignoreUnused(x);
        juce::ignoreUnused(y);

        if (slider.isRotary()) {
            layout.textBoxBounds = designHelper.subRectangle(slider.getLocalBounds(), 22, 24, 57, 20);
            layout.sliderBounds = designHelper.subRectangle(slider.getLocalBounds(), 0, 0, 100, 184);

        } else {
            layout.textBoxBounds = juce::Rectangle<int>(static_cast<int>(w * 0.3), 0, static_cast<int>(w * 0.4), static_cast<int>(h * 0.56));
            layout.sliderBounds = juce::Rectangle<int>(0, 0, w, h);
        }

        return layout;
    }
    
    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        JollLabel* label = new JollLabel;

        label->setColour (juce::Label::textColourId, juce::Colours::white);
        label->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite);
        label->setColour (juce::Label::backgroundColourId, juce::Colours::transparentWhite );

        label->setColour (juce::Label::textWhenEditingColourId, juce::Colours::white);
        label->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
        label->setColour (juce::Label::backgroundWhenEditingColourId, juce::Colours::transparentWhite );

        label->setJustificationType(juce::Justification::centred);
        //label->setFont(designHelper.robotoRegularFont.withPointHeight(sf(12)));
        label->setFont(designHelper.robotoRegularFont.withPointHeight(sf(12)));
        //label->setFont(designHelper.robotoRegularFont);

        // label->onEditorShow = []() { // thebug ("editor shown"); };
        // label->onEditorHide = []() { // thebug ("editor hidden"); };
        // label->onTextChange = []() { // thebug("Text changed"); };

        label->setLookAndFeel(&slider.getLookAndFeel());
        // label->setTooltip("Hello There General Kenobi");

        bool editOnSingleClick = false;
        bool editOnDoubleClick = false;
        bool lossOfFocusDiscardsChanges = true;
        label->setEditable (editOnSingleClick, editOnDoubleClick, lossOfFocusDiscardsChanges);
        
        return label;
    }
    
    void drawInnerShadow(juce::Graphics &g, juce::Path target, juce::Colour shadowColour, int radius, juce::Point<int> offset)
    {
        if (WOODEN_PC) {}
        else {
     

        // resets the Clip Region when the function returns
        juce::Graphics::ScopedSaveState saveState(g);
        
        // invert the path's fill shape and enlarge it,
        // so it casts a shadow
        juce::Path shadowPath(target);
        shadowPath.addRectangle(target.getBounds().expanded(10));
        shadowPath.setUsingNonZeroWinding(false);
        
        // reduce clip region to avoid the shadow
        // being drawn outside of the shape to cast the shadow on
        g.reduceClipRegion(target);

        // Radius = spread
        juce::DropShadow ds(shadowColour, radius, offset);
        ds.drawForPath(g, shadowPath);
        
        }
    }
    
    void drawInnerShadow(juce::Graphics &g, juce::Rectangle<int> rectangle, juce::Colour shadowColour, int radius, juce::Point<int> offset)
    {
        juce::Path path;
        path.addRectangle(rectangle);
        drawInnerShadow(g, path, shadowColour, radius, offset);
    }
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        juce::ignoreUnused(minSliderPos);
        juce::ignoreUnused(maxSliderPos);

        // slider.lookAndFeelChanged();

        if (style == juce::Slider::LinearHorizontal)
        {
            // Move this out and into const when resized
            juce::Rectangle<int> linearSliderBoundsRectangle (x, y, width, height);
            //linearSliderBoundsRectangle.setBounds(x, y, width, height);

            juce::Rectangle<int> linearSliderLabelRectangle  (x, y, width, static_cast<int>(height * 0.56f));
            juce::Rectangle<int> linearSliderValueRectangle  (x, y, width, static_cast<int>(height * 0.56f));
            juce::Rectangle<int> linearSliderTrackRectangle  (0, static_cast<int>(height * 0.64f), width, static_cast<int>(height * 0.24f));
            
            int x_ = std::max(static_cast<int>(1), static_cast<int>(width * 0.003571428571f));
            int y_ = std::max(static_cast<int>(1 + height * 0.64), static_cast<int>(height * 0.66f));
            juce::Rectangle<int> linearSliderHandleRectangle (x_, y_, juce::jmin((int)sliderPos, width-2), static_cast<int>(height * 0.2f));
            
            g.setColour(linearSliderBackgColor);
            g.fillRect(linearSliderBoundsRectangle);
            
            g.setColour(linearSliderLabelColor);
            g.setFont(designHelper.robotoRegularFont.withPointHeight(sf(12)));
            g.drawText(slider.getName(), linearSliderLabelRectangle, juce::Justification::left);
            
            g.setColour(linearSliderTrackColor);
            g.fillRect(linearSliderTrackRectangle);
            // drawInnerShadow(g, linearSliderTrackRectangle, juce::Colour(0,0,0), smi(1), {0,smi(1)});
            
            g.setColour(linearSliderHandleColor);
            g.fillRect(linearSliderHandleRectangle);
            
        }
    }


    juce::Path defaultRotaryArcPath(juce::Rectangle<int>& area, const float& rotaryStartAngle, const float& rotaryEndAngle)
    {
        juce::Path path;
        path.addCentredArc(
                static_cast<float>(area.getCentreX()), static_cast<float>(area.getCentreY()),
                static_cast<float>(area.getWidth()/2), static_cast<float>(area.getHeight()/2),
                0, rotaryStartAngle, rotaryEndAngle, true);
        return path;
    }

    juce::Path circle(const juce::Rectangle<int> &area)
    {
        juce::Path path;
        path.addRoundedRectangle(area, 9999);
        return path;
    }


    juce::Path circle(const juce::Rectangle<float> &area)
    {
        juce::Path path;
        path.addRoundedRectangle(area, 9999);
        return path;
    }


    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsMain  = juce::Rectangle<int> (x, y, width, height);
        auto boundsLabel = designHelper.rectangleFloatFromInt(getSliderLayout(slider).textBoxBounds);
        auto boundsTrack = designHelper.subRectangle(boundsMain, 16, 43+16, 68, 68);
        auto boundsKnob  = designHelper.subRectangle(boundsMain, 23, 43+23, 54, 54);
        auto boundsName  = designHelper.subRectangle(boundsMain, 0, 43+93, 100, 28);

        // drawLabelBackground
        g.setColour(juce::Colour(44, 44, 46));
        juce::Path sp;
        g.fillRoundedRectangle(boundsLabel, sf(2));
        sp.addRoundedRectangle(boundsLabel, sf(2));
        drawInnerShadow(g, sp, juce::Colours::black.withAlpha(0.25f), si(2), juce::Point(0, si(2)));

        // drawRotaryTrack
        auto trackPath = defaultRotaryArcPath(boundsKnob, rotaryStartAngle, rotaryEndAngle);
        g.setColour(juce::Colour(44,44,46));
        g.strokePath(trackPath, juce::PathStrokeType(rotaryTrackWidth));
        drawInnerShadow(g, circle(boundsTrack), juce::Colours::black.withAlpha(0.5f), rotaryTrackShadowRadius, rotaryTrackShadowOffset);

        // drawRotaryTrackTrail
        float radius = juce::jmin (boundsKnob.getWidth(), boundsKnob.getHeight()) / 2.0f;
        float toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // If middle out
        float fromAngle = (rotaryStartAngle + rotaryEndAngle) / 2;

        juce::Path track;
        track.addCentredArc(
                static_cast<float>(boundsKnob.getCentreX()), 
            static_cast<float>(boundsKnob.getCentreY()),
            static_cast<float>(boundsKnob.getWidth() / 2), 
            static_cast<float>(boundsKnob.getHeight()/2),
                0.0f, fromAngle, toAngle, true);

        juce::Path trackAlwaysPainted;

        trackAlwaysPainted.startNewSubPath(
            static_cast<float>(boundsKnob.getCentreX()),
            static_cast<float>(boundsKnob.getCentreY() - radius));

        trackAlwaysPainted.lineTo(
            static_cast<float>(boundsKnob.getCentreX()),
            static_cast<float>(boundsKnob.getCentreY() - radius - sf(6)));


        juce::GlowEffect glow;
        glow.setGlowProperties(sf(3), juce::Colours::white.withAlpha(0.2f), {0,0});
        juce::Image tempImage(juce::Image::ARGB, width, height, true);
        juce::Graphics glowGraphics(tempImage);
        glowGraphics.setColour(juce::Colours::white);
        glowGraphics.strokePath(track, juce::PathStrokeType(rotaryTrackWidth));
        glowGraphics.strokePath(trackAlwaysPainted, juce::PathStrokeType(sf(2), juce::PathStrokeType::curved, juce::PathStrokeType::square));
        glow.applyEffect(tempImage, g, 1, 1);

        // drawRotaryKnob
        juce::Path knobPath = circle(boundsKnob);
        juce::DropShadow dropShadow(juce::Colours::black.withAlpha(0.5f), 8, {0, si(8)});
        dropShadow.drawForPath(g, knobPath);
        g.setColour(juce::Colour(72, 72, 74));
        g.fillPath(knobPath);

        // drawRotaryThumb
        auto arcRadius = radius - rotaryThumbWidth * 0.5f;
        juce::Path thumbPath;
        thumbPath.startNewSubPath(
            static_cast<float>(boundsKnob.getCentreX() + ((arcRadius * 0.7) * std::cos(toAngle - juce::MathConstants<float>::halfPi))),
            static_cast<float>(boundsKnob.getCentreY() + ((arcRadius * 0.7) * std::sin(toAngle - juce::MathConstants<float>::halfPi))));

        thumbPath.lineTo(
            static_cast<float>(boundsKnob.getCentreX() + (arcRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi))),
            static_cast<float>(boundsKnob.getCentreY() + (arcRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi))));

        g.setColour(juce::Colour(44, 44, 46));
        g.strokePath(thumbPath, juce::PathStrokeType(rotaryThumbWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square));

        // drawRotaryKnobEdge
        designHelper.setGradientVertical(g, boundsKnob,
                juce::Colours::white.withAlpha(0.2f), juce::Colours::black.withAlpha(0.2f));
        g.drawRoundedRectangle(designHelper.rectangleFloatFromInt(boundsKnob), 999.0f, 1.0f);

        // drawRotaryName
        g.setColour(juce::Colour(170, 170, 170));
        g.setFont(designHelper.robotoRegularFont.withPointHeight(sf(12)));
        g.drawText(slider.getName(), boundsName, juce::Justification::centred, false);
    }


    void drawLabel (juce::Graphics& g, juce::Label& label) override
    {

        if (! label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font (getLabelFont (label));

            g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
            g.setFont (designHelper.robotoRegularFont);

            auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());

            g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                    juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                    label.getMinimumHorizontalScale());

            //g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
        }
        else if (label.isEnabled())
        {
            //g.setColour (label.findColour (juce::Label::outlineColourId));
        }

    }



    void drawDocumentWindowTitleBar (juce::DocumentWindow& window,
            juce::Graphics& g, int w, int h, int titleSpaceX, int titleSpaceW,
            const juce::Image* icon, bool drawTitleTextOnLeft) override
    {
        // thebug("DrawDocumentWindowTitleBar called");

        if (w * h == 0)
            return;

        auto isActive = window.isActiveWindow();

        g.fillAll(juce::Colour(40,40,40));

        juce::Font font ((float) h * 0.65f, juce::Font::plain);
        g.setFont (font);

        auto textW = font.getStringWidth (window.getName());
        auto iconW = 0;
        auto iconH = 0;

        if (icon != nullptr)
        {
            iconH = static_cast<int> (font.getHeight());
            iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
        }

        textW = juce::jmin (titleSpaceW, textW + iconW);
        auto textX = drawTitleTextOnLeft ? titleSpaceX
                : juce::jmax (titleSpaceX, (w - textW) / 2);

        if (textX + textW > titleSpaceX + titleSpaceW)
            textX = titleSpaceX + titleSpaceW - textW;

        if (icon != nullptr)
        {
            g.setOpacity (isActive ? 1.0f : 0.6f);
            g.drawImageWithin (*icon, textX, (h - iconH) / 2, iconW, iconH,
                    juce::RectanglePlacement::centred, false);
            textX += iconW;
            textW -= iconW;
        }

        if (window.isColourSpecified (juce::DocumentWindow::textColourId) || isColourSpecified (juce::DocumentWindow::textColourId))
            g.setColour (window.findColour (juce::DocumentWindow::textColourId));
        else
            g.setColour (getCurrentColourScheme().getUIColour (ColourScheme::defaultText));

        g.drawText (window.getName(), textX, 0, textW, h, juce::Justification::centredLeft, true);
    }

};
