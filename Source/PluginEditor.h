#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Monster.cpp"


class MonsterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MonsterAudioProcessorEditor (MonsterAudioProcessor&);
    ~MonsterAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    float scale = 0.7f;
    int width = 400;
    int height = 760;

    juce::Slider scaleSlider;
    JollLookAndFeel laf;
    MonsterAudioProcessor& audioProcessor;
    std::unique_ptr<Monster> monster;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonsterAudioProcessorEditor)
};
