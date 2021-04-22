/*
  ==============================================================================

    Monster.h
    Created: 24 Aug 2020 1:15:46pm
    Author:  admin

  ==============================================================================
*/

//#pragma once

#include <JuceHeader.h>
//#include "MonsterLookAndFeels.h"
#include "PluginProcessor.h"
#include "JollLookAndFeel.h"
#include "Background.h"
#include "Container.h"
#include "phaseComponent.h"
#include "JollSlider.h"


class Monster  : public juce::Component
{
private:
    const float designWidth = 400;
    const float designHeight = 760;
    const float indent = 8;
    float widthScale = 1;
    float heightScale = 1;
    float scale = 1;

    MonsterAudioProcessor& audioProcessor;
    Background background;
    Container monoContainer;
    Container phaseContainer;
    Container sliderContainer;
    Container rotaryContainer;

    PhaseComponent phaseComponent;
    
    juce::ToggleButton monoButton, lPhaseButton, rPhaseButton;


    juce::Slider widthHighSlider,
        xoverSlider,
        widthLowSlider,
        gainSlider,
        leftPanSlider,
        rightPanSlider;
    juce::Slider inputBalanceSlider;

    JollLookAndFeel jollLAF;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "MONO", monoButton);

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseLeftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "PHASE_LEFT", lPhaseButton);

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaseRightAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "PHASE_RIGHT", rPhaseButton);

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "INPUT_BALANCE", inputBalanceSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "LEFT_PAN", leftPanSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RIGHT_PAN", rightPanSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WIDTH_HI", widthHighSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthLoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WIDTH_LO", widthLowSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xoverAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "XOVER", xoverSlider);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "GAIN", gainSlider);

    int w, h;

    
public:
    Monster(MonsterAudioProcessor& p) : audioProcessor (p)
    {

        addAndMakeVisible(&background);
        addAndMakeVisible(&monoContainer);
        addAndMakeVisible(&phaseContainer);
        addAndMakeVisible(&rotaryContainer);
        addAndMakeVisible(&sliderContainer);
        
        monoButton.setLookAndFeel(&jollLAF);
        monoButton.setComponentID("mono");
        monoButton.setWantsKeyboardFocus(false);
        addAndMakeVisible(&monoButton);

        addAndMakeVisible(&phaseComponent);

        lPhaseButton.setLookAndFeel(&jollLAF);
        lPhaseButton.setComponentID("phase");
        lPhaseButton.setWantsKeyboardFocus(false);
        addAndMakeVisible(lPhaseButton);

        rPhaseButton.setLookAndFeel(&jollLAF);
        rPhaseButton.setComponentID("phase");
        rPhaseButton.setWantsKeyboardFocus(false);
        addAndMakeVisible(&rPhaseButton);

        gainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        gainSlider.setLookAndFeel(&jollLAF);
        //gainSlider.setDoubleClickReturnValue(true, 0);
        gainSlider.setTextValueSuffix(juce::String(" dB"));
        gainSlider.setName("Gain");
        addAndMakeVisible(&gainSlider);

        xoverSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        xoverSlider.setTooltip(juce::String("This is a tooltip for crossover."));
        xoverSlider.setLookAndFeel(&jollLAF);
        xoverSlider.setDoubleClickReturnValue(true, 200.0f);
        xoverSlider.setName("Crossover");
        addAndMakeVisible(&xoverSlider);

        widthLowSlider.setLookAndFeel(&jollLAF);
        widthLowSlider.setDoubleClickReturnValue(true, 1);
        widthLowSlider.setTextValueSuffix(juce::String(" %"));
        widthLowSlider.setName("Width low");
        addAndMakeVisible(&widthLowSlider);

        widthHighSlider.setLookAndFeel(&jollLAF);
        widthHighSlider.setDoubleClickReturnValue(true, 1);
        widthHighSlider.setTextValueSuffix(juce::String(" %"));
        widthHighSlider.setName("Width high");
        addAndMakeVisible(&widthHighSlider);

        inputBalanceSlider.setNumDecimalPlacesToDisplay(0);
        inputBalanceSlider.setDoubleClickReturnValue(true, 1.0f);
        inputBalanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        inputBalanceSlider.setName("Input balance");
        inputBalanceSlider.setLookAndFeel(&jollLAF);
        addAndMakeVisible(&inputBalanceSlider);

        leftPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        leftPanSlider.setDoubleClickReturnValue(true, 0.5f);
        leftPanSlider.setName("Pan left");
        leftPanSlider.setLookAndFeel(&jollLAF);
        addAndMakeVisible(&leftPanSlider);

        rightPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        rightPanSlider.setPaintingIsUnclipped(true);
        rightPanSlider.setDoubleClickReturnValue(true, 0.5f);
        rightPanSlider.setName("Pan right");
        rightPanSlider.setLookAndFeel(&jollLAF);
        addAndMakeVisible(&rightPanSlider);
    }

    ~Monster() override
    {
        monoButton.setLookAndFeel(nullptr);
        lPhaseButton.setLookAndFeel(nullptr);
        rPhaseButton.setLookAndFeel(nullptr);
        inputBalanceSlider.setLookAndFeel(nullptr);
        leftPanSlider.setLookAndFeel(nullptr);
        rightPanSlider.setLookAndFeel(nullptr);
        widthHighSlider.setLookAndFeel(nullptr);
        xoverSlider.setLookAndFeel(nullptr);
        widthLowSlider.setLookAndFeel(nullptr);
        gainSlider.setLookAndFeel(nullptr);
    }


    void resized() override
    {
        inputBalanceSlider.setLookAndFeel(&jollLAF);


        w = getWidth();
        h = getHeight();
        widthScale = getWidth() / designWidth;
        heightScale = getHeight() / designHeight;
        scale = 0.5f * (widthScale + heightScale);
        // thebug("WidthScale", widthScale, "heightScale", heightScale, "width", w, "height", h, "scale", scale);

        jollLAF.resize();

        auto mainBackgroundRectangle = getLocalBounds();
        auto mainContentRectangle = designHelper.subRectangle(mainBackgroundRectangle, 7, 8, 384, 744);

        auto monoContainerRectangle   = designHelper.subRectangle(mainContentRectangle, 0, 0,   384, 136);
        auto phaseContainerRectangle  = designHelper.subRectangle(mainContentRectangle, 0, 144, 384, 80);
        auto rotaryContainerRectangle = designHelper.subRectangle(mainContentRectangle, 0, 232, 384, 188);
        auto sliderContainerRectangle = designHelper.subRectangle(mainContentRectangle, 0, 428, 384, 316);

        auto monoStackRectangle   = monoContainerRectangle;
        // auto phaseStackRectangle  = phaseContainerRectangle;
        auto rotaryStackRectangle = designHelper.subRectangle(rotaryContainerRectangle, 42, 9,  300, 169);
        auto sliderStackRectangle = designHelper.subRectangle(sliderContainerRectangle, 52, 33, 280, 248);

        background.setBounds(mainBackgroundRectangle);
        monoContainer.setBounds(monoContainerRectangle);
        phaseContainer.setBounds(phaseContainerRectangle);
        rotaryContainer.setBounds(rotaryContainerRectangle);
        sliderContainer.setBounds(sliderContainerRectangle);

        monoButton.setBounds(monoStackRectangle);

        phaseComponent.setBounds   (phaseContainerRectangle);
        lPhaseButton.setBounds (designHelper.subRectangle(phaseContainerRectangle, 124, 10, 60, 60));
        rPhaseButton.setBounds (designHelper.subRectangle(phaseContainerRectangle, 184, 10, 60, 60));

        leftPanSlider.setBounds      (designHelper.subRectangle(rotaryStackRectangle, 0,   0, 100, 169));
        inputBalanceSlider.setBounds (designHelper.subRectangle(rotaryStackRectangle, 100, 0, 100, 169));
        rightPanSlider.setBounds     (designHelper.subRectangle(rotaryStackRectangle, 200, 0, 100, 169));

        gainSlider.setBounds      (designHelper.subRectangle(sliderStackRectangle, 0, 0, 280, 50));
        xoverSlider.setBounds     (designHelper.subRectangle(sliderStackRectangle, 0, 66, 280, 50));
        widthHighSlider.setBounds (designHelper.subRectangle(sliderStackRectangle, 0, 132, 280, 50));
        widthLowSlider.setBounds  (designHelper.subRectangle(sliderStackRectangle, 0, 198, 280, 50));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monster)
};
