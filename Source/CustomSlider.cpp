/*
 ==============================================================================
 
 CustomSlider.cpp
 Created: 2 Sep 2016 12:59:30pm
 Author:  Armando Montanez
 
 ==============================================================================
 */

#include "CustomSlider.h"
void CustomSlider::paint(Graphics& g)
{
    int offset = 267-(int)(getValue()*267);
    
    g.drawImage(handle,0,offset, handle.getWidth(), handle.getHeight(), 0, 0, handle.getWidth(), handle.getHeight());
}