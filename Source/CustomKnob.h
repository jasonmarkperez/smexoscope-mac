/*
 ==============================================================================
 
 CustomKnob.h
 Created: 31 Aug 2016 11:36:16am
 Author:  Armando Montanez
 
 ==============================================================================
 
 This knob class assumes square knob images, dynamically determining other
 properties about the filmstrip.
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef CUSTOMKNOB_H_INCLUDED
#define CUSTOMKNOB_H_INCLUDED

class CustomKnob : public Slider
{
public:
    CustomKnob(Image raw, bool vertical);
    void paint(Graphics& g) override;
private:
    // dimensions of our knob slide
    int dimensions;
    int numFrames;
    bool isVertical;
    Image sequence;
    
};



#endif  // CUSTOMKNOB_H_INCLUDED
