/*
 ==============================================================================
 
 JuceMultiStateButton.cpp
 Created: 2 Sep 2016 10:25:57am
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "JuceMultiStateButton.h"

JuceMultiStateButton::JuceMultiStateButton(Image raw, bool vertical, int numstates)
: isVertical(vertical), sequence(raw)
{
    if (isVertical)
    {
        width = sequence.getWidth();
        numFrames = numstates;
        if (sequence.getHeight() % numFrames != 0)
        {
            // error, wrong image dimensions.
        }
        else
            height = sequence.getHeight()/numFrames;
        
    }
    else
    {
        height = sequence.getHeight();
        numFrames = numstates;
        if (sequence.getWidth() % numFrames != 0)
        {
            // error, wrong image dimensions.
        }
        else
            width = sequence.getWidth()/numFrames;
    }
    
}

void JuceMultiStateButton::paint(Graphics& g)
{
    if (isVertical)
    {
        g.drawImage(sequence,0,0, width, height, 0, index*height, width, height);
    }
    else
    {
        
        g.drawImage(sequence,0,0, width, height, index*width, 0, width, height);
    }
}

void JuceMultiStateButton::mouseDown(const MouseEvent &event)
{
    if (!mouseWasHandled)
    {
        setValue(++index);
        mouseWasHandled = true;
        this->repaint();
    }
}
void JuceMultiStateButton::mouseUp(const MouseEvent &event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this)
        mouseWasHandled = false;
}