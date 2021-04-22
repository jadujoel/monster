#include "PluginEditor.h"

MonsterAudioProcessorEditor::MonsterAudioProcessorEditor (MonsterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    monster = std::make_unique<Monster>(p);
    
    
//    juce::Rectangle<int> r = juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    // juce::DocumentWindow
    
    int screen_height = juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
//    const int h = static_cast<int>(screen_height * 0.7);
//    setSize (static_cast<int>(h*0.6), h);
    //setSize (400*0.9, 760*0.9);


    scale = 1;
    width = 400;
    height = 760;
    designHelper.setDesignSize(static_cast<float>(width), static_cast<float>(height), 12.0f);

    std::unique_ptr<juce::Label> valueBox;
    //valueBox.get()->setFont(<#const Font & newFont#>);

    // auto screenBounds = getScreenBounds();
    // // thebug("ScreenBounds", screenBounds.getX(), screenBounds.getY(), screenBounds.getWidth(), screenBounds.getHeight());
   
    
//
//    scaleSlider.setBounds(0, 0, 200, 20);
//    scaleSlider.setRange(0.5, 2.0, 0.1);
    
    // thebug("screen height", screen_height);
    
//    scaleSlider.onValueChange = [this]() {
//        this->scale = scaleSlider.getValue();
//        this->resized(); };
//    scaleSlider.setLookAndFeel(&laf);
//    addAndMakeVisible(&scaleSlider);

    float heightWithOverhead = static_cast<float>(height)+160.0f;
    
    //setResizable(true, true);
    //setResizable(<#bool allowHostToResize#>, <#bool useBottomRightCornerResizer#>)
    
    //setResizeLimits(<#int newMinimumWidth#>, <#int newMinimumHeight#>, <#int newMaximumWidth#>, <#int newMaximumHeight#>)
    //res
    
    if (screen_height < heightWithOverhead) {
        //scaleSlider.setValue(screen_height / heightWithOverhead, juce::NotificationType::dontSendNotification);
        //scale = static_cast<float>(screen_height) / heightWithOverhead;
        //setScaleFactor(static_cast<float>(screen_height) / heightWithOverhead);
        scale = screen_height / heightWithOverhead;
    }
    else {
        //scaleSlider.setValue(1.0, juce::NotificationType::dontSendNotification);
        scale = 1.0f;
    }
    
   
    setSize (static_cast<int>(width * scale), static_cast<int>(height * scale));
    designHelper.setBounds(getLocalBounds());
    monster->setBounds(getLocalBounds());
    addAndMakeVisible(monster.get());
    

}

MonsterAudioProcessorEditor::~MonsterAudioProcessorEditor()
{
    scaleSlider.setLookAndFeel(nullptr);
    monster = nullptr;

}

//==============================================================================
void MonsterAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void MonsterAudioProcessorEditor::resized()
{
    
    //designHelper.rescale(scale);
    //designHelper.setBounds(getLocalBounds());
    //setSize (width * scale, height * scale);
    monster->setBounds(getLocalBounds());
}


