
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// #ifndef = headerguard to prevent C++ header files from being included multiple times
MonsterAudioProcessor::MonsterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor ( BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
mAPVTS ( *this, nullptr, "PARAMETERS", createParameters() )
#endif
{
    mAPVTS.state.addListener(this);
    mAPVTS.state = juce::ValueTree("savedParams");
    setLatencySamples(0);
}

MonsterAudioProcessor::~MonsterAudioProcessor()
{
}

//==============================================================================
const juce::String MonsterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MonsterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MonsterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MonsterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MonsterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MonsterAudioProcessor::getNumPrograms()
{
    return 1;
}

int MonsterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MonsterAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String MonsterAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MonsterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void MonsterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ps.sampleRate = sampleRate;
    ps.maximumBlockSize = samplesPerBlock;
    ps.numChannels = 2;
    // thebug("Prepare to play");
    // thebug("  Sample rate ", ps.sampleRate);
    // thebug("  Max block size ", ps.maximumBlockSize);
    // thebug("  Num channels ", ps.numChannels);
    bandsplit.prepare(ps);
    updateParameters();
}

void MonsterAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MonsterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MonsterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateParameters();

//    rmsInput = buffer.getRMSLevel(0, 0, buffer.getNumSamples()) + buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    
    if (totalNumInputChannels == 2)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            signalLeft  = buffer.getSample(0, sample);
            signalRight = buffer.getSample(1, sample);
            
            // Input Balance
            signalLeft  *= balanceL;
            signalRight *= balanceR;
            
            // Phase swap
            if (mParams.phaseLeft) { signalLeft  *= -1; }
            if (mParams.phaseRight){ signalRight *= -1; }

            // Bandsplit
            bandsplit.processSample(0, signalLeft, loLeft, hiLeft);
            bandsplit.processSample(1, signalRight, loRight, hiRight);
            
            // Width High Freq
            temp    = hiLeft  + (hiRight * oneMinusWidthHi);
            hiRight = hiRight + (hiLeft  * oneMinusWidthHi);
            hiLeft  = temp;
            
            // Width Low Freq
            temp    = loLeft  + (loRight * oneMinusWidthLo);
            loRight = loRight + (loLeft  * oneMinusWidthLo);
            loLeft  = temp;
            
            // Pan law adjustment
            loLeft  *= lawAdjustLo;
            loRight *= lawAdjustLo;
            hiLeft  *= lawAdjustHi;
            hiRight *= lawAdjustHi;
            
            // Recombine low/high freqs
            signalLeft  = loLeft + hiLeft;
            signalRight = loRight + hiRight;
        
            // Mono summing
            if (mParams.mono) { signalLeft = signalRight = panLaw * (signalLeft + signalRight); }

            // Pan
            temp        = signalLeft;
            signalLeft  = signalLeft * (1.0f-mParams.left_pan) + signalRight * (1.0f-mParams.right_pan);
            signalRight = signalRight * mParams.right_pan + (temp * mParams.left_pan);

            // Gain
            signalLeft  = signalLeft  * juce::Decibels::decibelsToGain(mParams.gain);
            signalRight = signalRight * juce::Decibels::decibelsToGain(mParams.gain);

            buffer.setSample(0, sample, signalLeft);
            buffer.setSample(1, sample, signalRight);
        }
    }
//    rmsOutput = buffer.getRMSLevel(0, 0, buffer.getNumSamples()) + buffer.getRMSLevel(1, 0, buffer.getNumSamples());
//    gainCompensationPrevious = gainCompensation;
//    gainCompensation = fmin(20, rmsInput / fmax(rmsOutput, 0.01));
//    buffer.applyGain(gainCompensation);
//    buffer.applyGainRamp(0, buffer.getNumSamples(), gainCompensationPrevious, gainCompensation);
//    std::cout << gainCompensation << std::endl;
//    std::cout << buffer.getMagnitude(0, buffer.getNumSamples()) << " ";
}

//==============================================================================
bool MonsterAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* MonsterAudioProcessor::createEditor()
{
    return new MonsterAudioProcessorEditor (*this);
}

//==============================================================================
void MonsterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto xml = std::unique_ptr<juce::XmlElement>(mAPVTS.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MonsterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto savedParams = std::unique_ptr<juce::XmlElement>(getXmlFromBinary(data, sizeInBytes));
    if ((savedParams != nullptr) && (savedParams->hasTagName(mAPVTS.state.getType())))
    { mAPVTS.state = juce::ValueTree::fromXml(*savedParams); }
}

juce::AudioProcessorValueTreeState::ParameterLayout MonsterAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    /*
      parameters.push_back(std::make_unique<juce::AudioParameterInt>(
                    "MONO", "Mono",
                    0, 1,
                    0,
                    juce::String(),
                    [](int value, int) { return juce::String(value);},
                    [](const juce::String &text) { return text.getIntValue(); }));
    
     */

    parameters.push_back(std::make_unique<juce::AudioParameterBool> ("MONO",          "Mono",        false, juce::String()));
    parameters.push_back(std::make_unique<juce::AudioParameterBool> ("PHASE_LEFT",    "Phase Left",  false, juce::String()));
    parameters.push_back(std::make_unique<juce::AudioParameterBool> ("PHASE_RIGHT",   "Phase Right", false, juce::String()));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
            "INPUT_BALANCE", "Input Balance",// ParameterID, ParameterName
            juce::NormalisableRange<float>(0.0f, 2.0f), 1.0f, //defaultValue
            juce::String("BalanceParameter"), // ParameterLabel
            juce::AudioProcessorParameter::genericParameter, // parameterCategory
            [] (float value, int) { // Display Value
                return juce::String (static_cast<int> ((value - 1) * 100));
            },
            [] (const juce::String &text) { // Value from user text input
                return text.getFloatValue() / 100 + 1;
            }
            )
    );

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
            "LEFT_PAN", "Left Pan", // ParameterID, ParameterName
            juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f, //defaultValue
            juce::String("LeftPanParameter"), // ParameterLabel
            juce::AudioProcessorParameter::genericParameter, // parameterCategory
            [] (float value, int) {
                return juce::String (static_cast<int> ((value-0.5) * 200));
            },
            [] (const juce::String &text) {
                return text.getFloatValue() / 200.0f + 0.5f;
            }
            )
    );

    //parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RIGHT_PAN",     "Right Pan",     0.0f, 1.0f, 1.0f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
            "RIGHT_PAN", "Right Pan", // ParameterID, ParameterName
            juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f, //defaultValue
            juce::String("RightPanParameter"), // ParameterLabel
            juce::AudioProcessorParameter::genericParameter, // parameterCategory
            [] (float value, int) {
                return juce::String (static_cast<int> ((value-0.5f) * 200.0f));
            },
            [] (const juce::String &text) {
                return text.getFloatValue() / 200.0f + 0.5f;
            }
            )
    );

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "WIDTH_HI", "Width Hi",// ParameterID, ParameterName
        juce::NormalisableRange<float>(0.0f, 2.0f), 1.0f, //defaultValue
        juce::String("WidthParameter"), // ParameterLabel
        juce::AudioProcessorParameter::genericParameter, // parameterCategory
        [] (float value, int) {
            return juce::String (static_cast<int> (value * 100));
        },
        [] (const juce::String &text) {
            return text.getFloatValue() / 100.0f;
        }
        )
    );
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        "WIDTH_LO", "WidthLo",// ParameterID, ParameterName
        juce::NormalisableRange<float>(0.0f, 2.0f),
        1.0f, //defaultValue
        juce::String("WidthLoParameter"), // ParameterLabel
        juce::AudioProcessorParameter::genericParameter, // parameterCategory
        [] (float value, int) {
            return juce::String (static_cast<int> (value * 100));
        },
        [] (const juce::String &text) {
            return text.getFloatValue() / 100.0f;
        })
    );
    
    //    https://docs.juce.com/master/classSlider.html
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     "XOVER", "Xover",                      // ParameterID, ParameterName
                                                                     juce::NormalisableRange<float>(20.0f,  //ValueType rangeStart,
                                                                                                    20000.0f,           //ValueType rangeEnd,
                                                                                                    1.0f,               //ValueType intervalValue,
                                                                                                    0.2f,                //ValueType skewFactor,
                                                                                                    false),              //bool useSymmetricSkew = false)
                                                                     20.0f, //defaultValue
                                                                     juce::String("XoverParameter"), // ParameterLabel
                                                                     juce::AudioProcessorParameter::genericParameter, // parameterCategory
                                                                     [](float value, int) { return juce::String(value, 0) + juce::String(" Hz"); }, // What Value should be displayd
                                                                     
                                                                     [](const juce::String &text) { return text.getFloatValue(); } // Value from user text input
                                                                     ));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", -12.0f, 12.0f, 0.0f));
    
    return { parameters.begin(), parameters.end() };
}

void MonsterAudioProcessor::updateParameters()
{
    mParams.mono          = mAPVTS.getRawParameterValue("MONO")->load();
    mParams.phaseLeft     = mAPVTS.getRawParameterValue("PHASE_LEFT")->load();
    mParams.phaseRight    = mAPVTS.getRawParameterValue("PHASE_RIGHT")->load();
    mParams.input_balance = mAPVTS.getRawParameterValue("INPUT_BALANCE")->load();
    mParams.left_pan      = mAPVTS.getRawParameterValue("LEFT_PAN")->load();
    mParams.right_pan     = mAPVTS.getRawParameterValue("RIGHT_PAN")->load();
    mParams.widthHi       = mAPVTS.getRawParameterValue("WIDTH_HI")->load();
    mParams.widthLo       = mAPVTS.getRawParameterValue("WIDTH_LO")->load();
    mParams.xover         = mAPVTS.getRawParameterValue("XOVER")->load();
    mParams.gain          = mAPVTS.getRawParameterValue("GAIN")->load();
    //std::cout << " Mono: " << mParams.mono << " phL: " << mParams.phaseLeft << " phR: " << mParams.phaseRight << " Swap: " << mParams.swap << " Balance: " << mParams.balance << " Width " << mParams.width << std::endl;
    
    absWidthHi      = abs(mParams.widthHi);
    absWidthLo      = abs(mParams.widthLo);
    oneMinusWidthHi = 1.0f - absWidthHi;
    oneMinusWidthLo = 1.0f - absWidthLo;
    lawAdjustHi     = panLaw + absWidthHi * (1.0f - panLaw);
    lawAdjustLo     = panLaw + absWidthLo * (1.0f - panLaw);
    
    balanceR = std::min<float>(     mParams.input_balance, 1.0f);
    balanceL = std::min<float>(2.0f-mParams.input_balance, 1.0f);
    bandsplit.setCutoffFrequency (mParams.xover);
    //std::cout << "balance " << balanceL << " " << balanceR << std::endl;

}
    
void MonsterAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
 {
    juce::ignoreUnused(treeWhosePropertyHasChanged);
    juce::ignoreUnused(property);
    updateParameters(); 
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new MonsterAudioProcessor(); }
