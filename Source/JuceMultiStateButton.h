/*
 ==============================================================================
 
 JuceMultiStateButton.h
 Created: 2 Sep 2016 10:25:47am
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef JUCEMULTISTATEBUTTON_H_INCLUDED
#define JUCEMULTISTATEBUTTON_H_INCLUDED

class JuceMultiStateButton : public Component
{
public:
    JuceMultiStateButton(Image raw, bool vertical, int numstates);
    void paint(Graphics& g) override;
    
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseUp(const MouseEvent &event) override;
    
    // various getters/setters of little consequence other than crucial functionality
    void setNumModes(int i) { numFrames = i; }
    int getValue() { return index; }
    float getFloatValue() { return value; }
    void setRange(float floor, float ceil, float step)
    {
        min = floor;
        max = ceil;
        stepsize = step;
    }
    void setValue(int x)
    {
        if (x >= numFrames)
            index = 0;
        else if (x < 0)
            index = 0;
        else
            index = x;
        value = index*stepsize;
    }
    void setValue(float x)
    {
        value = x;
        index = x/stepsize;
    }
private:
    // dimensions of our knob slide
    int width;
    int height;
    int index;
    float min;
    float max;
    float stepsize;
    float value;
    int numFrames;
    bool isVertical;
    bool mouseWasHandled = false;
    Image sequence;
    
};



#endif  // JUCEMULTISTATEBUTTON_H_INCLUDED
