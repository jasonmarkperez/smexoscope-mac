/*
 ==============================================================================
 
 CustomSlider.h
 Created: 2 Sep 2016 12:59:30pm
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef CUSTOMSLIDER_H_INCLUDED
#define CUSTOMSLIDER_H_INCLUDED

class CustomSlider : public Slider
{
public:
    CustomSlider(Image raw) { handle = raw; }
    void paint(Graphics& g) override;
private:
    int offset;
    Image handle;
    
};



#endif  // CUSTOMSLIDER_H_INCLUDED
