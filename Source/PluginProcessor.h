/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// #include "JollLookAndFeel.h"

//==============================================================================
/**
*/
class MonsterAudioProcessor  : public juce::AudioProcessor, juce::ValueTree::Listener
{
public:

    //==============================================================================
    MonsterAudioProcessor();
    ~MonsterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState& getAPVTS() {return mAPVTS;}

    juce::dsp::LinkwitzRileyFilter<float> bandsplit;
    juce::dsp::ProcessSpec ps;
    
private:
    juce::AudioProcessorValueTreeState mAPVTS;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    float loLeft = 0.0f;
    float loRight = 0.0f;
    float hiLeft = 0.0f;
    float hiRight = 0.0f;
    
    float absWidthHi;
    float absWidthLo;
    float oneMinusWidthHi;
    float oneMinusWidthLo;
    float panLaw = 0.7079458f; // 3db
    float lawAdjustHi;
    float lawAdjustLo;
    float balanceL;
    float balanceR;
    float temp;
    
//    float rmsInput, rmsOutput, gainCompensation, gainCompensationPrevious;
    
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void updateParameters();
    
    class Parameters{ public:
        bool mono, phaseLeft, phaseRight;
        float input_balance, left_pan, right_pan, cutoff, widthHi, xover, widthLo, gain;
        
    }; Parameters mParams;
    
    float signal, signalLeft, signalRight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonsterAudioProcessor)
};
