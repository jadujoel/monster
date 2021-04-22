/*
  ==============================================================================

    JollLabel.h
    Created: 26 Feb 2021 4:09:28pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Debug.h"
#include "DesignHelper.h"


class JollLabel : public juce::Label
{
public:
    JollLabel(){}

    static void copyColourIfSpecified (juce::Label& l, juce::TextEditor& ed, int colourID, int targetColourID)
    {
        if (l.isColourSpecified (colourID) || l.getLookAndFeel().isColourSpecified (colourID))
            ed.setColour (targetColourID, l.findColour (colourID));
    }

    juce::TextEditor* createEditorComponent() override
    {
        auto* ed = new juce::TextEditor (getName());
        ed->applyFontToAllText (getLookAndFeel().getLabelFont (*this));
        copyAllExplicitColoursTo (*ed);

        copyColourIfSpecified (*this, *ed, textWhenEditingColourId, juce::TextEditor::textColourId);
        copyColourIfSpecified (*this, *ed, backgroundWhenEditingColourId, juce::TextEditor::backgroundColourId);
        copyColourIfSpecified (*this, *ed, outlineWhenEditingColourId, juce::TextEditor::focusedOutlineColourId);
        ed->setJustification(juce::Justification::centred);

        return ed;
    }

//    void resized()
//    {
//        if (editor != nullptr) {
//            editor->setBounds (getLocalBounds());
//        }
//
//        // thebug("Label Resize");
//    }

    void paint (juce::Graphics& g) override
    {
        setFont(designHelper.robotoRegularFont);
        getLookAndFeel().drawLabel (g, *this);
    }

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JollLabel)
};
