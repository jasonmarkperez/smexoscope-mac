/*
 ==============================================================================
 
 TextElement.cpp
 Created: 2 Sep 2016 12:16:25pm
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "TextElement.h"
void TextElement::paint(Graphics& g)
{
    g.setColour(juce::Colour(255, 255, 255));
    
    g.setFont(12.0f);
    
    Rectangle<int> textRect(0, 0, 30, 12);
    
    sprintf(text, "%.5f", value);
    g.drawText(text, textRect, Justification::centredTop, false);
}