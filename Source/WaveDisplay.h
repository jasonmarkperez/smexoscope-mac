#pragma once

#include "Defines.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "SmartelectronixDisplay.h"

class CWaveDisplay : public Component{
public:
    CWaveDisplay(Rectangle<int>& size, CSmartelectronixDisplay* effect, juce::Image back, juce::Image heads, juce::Image readout, double sampleRate);
    
    virtual ~CWaveDisplay();
    
    virtual void paint(Graphics& g) override;
    
    // custom draw function to draw an aliased line
    void drawAliasedLine(Graphics& g, int x1, int y1, int x2, int y2);
    
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseDrag(const MouseEvent &event) override;
    virtual void mouseUp(const MouseEvent &event) override;
    void setEffectParameter(int index, float value);
    
    int kLeftText = Justification::left;
    
protected:
    juce::Point<int> where;
    Rectangle<int>& size;
    
    CSmartelectronixDisplay* effect;
    
    juce::Image back;
    juce::Image heads;
    juce::Image readout;
    
    double sampleRate = 44100;
    
    unsigned char display;
};
