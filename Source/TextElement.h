/*
 ==============================================================================
 
 TextElement.h
 Created: 2 Sep 2016 12:16:25pm
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef TEXTELEMENT_H_INCLUDED
#define TEXTELEMENT_H_INCLUDED

class TextElement : public Component
{
public:
    TextElement(){}
    void paint(Graphics& g) override;
    
    void setValue(float x)
    {
        value = x;
        repaint();
    }
private:
    char text[256];
    float value = 0.0f;
};







#endif  // TEXTELEMENT_H_INCLUDED
