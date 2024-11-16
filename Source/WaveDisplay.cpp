// CustomVU.cpp: implementation of the CCustomVU class.
//
//////////////////////////////////////////////////////////////////////

#include "WaveDisplay.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../JuceLibraryCode/JuceHeader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// CVuMeter
//------------------------------------------------------------------------
CWaveDisplay::CWaveDisplay(Rectangle<int>& size, CSmartelectronixDisplay* effect, juce::Image back, juce::Image heads, juce::Image readout, double sampleRate)
: size(size)
, effect(effect)
, back(back)
, heads(heads)
, readout(readout)
, sampleRate(sampleRate)
{
    // this is the magic that selects which smartelectronix head to display in the corner of the wave display.
    srand((unsigned)time(NULL));
    display = rand() % 4;
    
    // link this display to our data block so we can communicate and get data
    effect->linkDisplay(this);
    
    // set saved mouse position to something invalid so crosshairs don't appear by default.
    where.x = -1;
}

//------------------------------------------------------------------------
CWaveDisplay::~CWaveDisplay()
{
    
}

//inline because we'd like to avoid the switching overhead
void inline CWaveDisplay::drawAliasedLine(Graphics& g, int x1, int y1, int x2, int y2)
{
    if (y2 > y1)
        g.fillRect(x1, y1, 1, y2-y1);
    else if (y1 > y2)
        g.fillRect(x1, y2, 1, y1-y2);
    else
        g.fillRect(x1, y1, 1, 1);
}

// basic setter to change saved parameter values from UI elements.
void CWaveDisplay::setEffectParameter(int index, float value)
{
    effect->setParameter(index, value);
}

// handle mouse clicking waveform display
void CWaveDisplay::mouseDown(const MouseEvent &event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this)
        this->where = event.getPosition();
}

// handle mouse draging over waveform display
void CWaveDisplay::mouseDrag(const MouseEvent &event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this)
    {
        // set position
        this->where = event.getPosition();
        
        // update such that position is limited to plugin bounds
        if (event.getPosition().x < 0)
            this->where.setX(0);
        else if(event.getPosition().x > size.getWidth())
            this->where.setX(size.getWidth()-1);
        if(event.getPosition().y < 0)
            this->where.setY(0);
        else if (event.getPosition().y > size.getHeight())
            this->where.setY(size.getHeight()-1);
    }
}

// handle mouse release over waveform display
void CWaveDisplay::mouseUp(const MouseEvent &event)
{
    // crosshairs "stick" until you right click
    if (event.mods.isRightButtonDown() && event.originalComponent == this)
        this->where.x = -1;
}

// this is a quick little helper function needed for text readout.
inline float cf_lin2db(float lin)
{
    if (lin < 9e-51)
        return -1000.f; /* prevent invalid operation */
    return 20.f * log10f(lin);
}

//------------------------------------------------------------------------
void CWaveDisplay::paint(Graphics& g)
{
    ///////////////////////////////////////////////////////
    // these are all the draw calls for the wave display //
    ///////////////////////////////////////////////////////
    //  offset of the waveform display should be 38, 16  //
    
    // draw a random face because why not
    g.drawImage(heads, 579, 224, 46, 45, 0, display*(heads.getHeight()/4), 46, 45);
    
    // just in case somebody was messing with opacity
    g.setOpacity(1.0f);
    
    // trig-line, grey
    long triggerType = (long)(effect->getParameter(CSmartelectronixDisplay::kTriggerType) * CSmartelectronixDisplay::kNumTriggerTypes + 0.0001);
    
    if (triggerType == CSmartelectronixDisplay::kTriggerRising || triggerType == CSmartelectronixDisplay::kTriggerFalling) {
        long y = 1 + (long)((1.f - effect->getParameter(CSmartelectronixDisplay::kTriggerLevel)) * (size.getHeight() - 2));
        
        g.setColour(juce::Colour(229, 229, 229));
        g.drawHorizontalLine(y, 0, size.getWidth() - 1);
    }
    
    // zero-line, orange
    g.setColour(juce::Colour(179, 111, 56));
    g.drawLine(0, size.getHeight() * 0.5 - 1, size.getWidth() - 1, size.getHeight() * 0.5 - 1);
    
    
    const std::vector<Point<int>>& points = (effect->getParameter(CSmartelectronixDisplay::kSyncDraw) > 0.5f) ? effect->getCopy() : effect->getPeaks();
    double counterSpeedInverse = pow(10.f, effect->getParameter(CSmartelectronixDisplay::kTimeWindow) * 5.f - 1.5);
    // waveform
    if (counterSpeedInverse < 1.0) //draw interpolated lines!
    {
        // set color to blue
        g.setColour(juce::Colour(64, 148, 172));
        
        double phase = counterSpeedInverse;
        double dphase = counterSpeedInverse;
        
        double prevxi = points[0].x;
        double prevyi = points[0].y;
        
        for (long i = 1; i < size.getWidth() - 1; i++) {
            long index = (long)phase;
            double alpha = phase - (double)index;
            
            double xi = i;
            double yi = (1.0 - alpha) * points[index * 2].y + alpha * points[(index + 1) * 2].y;
            
            // this line is renderd with AA, so it looks different (arguably better?) from the original s(M)exoscope.
            // use fillRectangle to solve.
            g.drawLine(prevxi, prevyi, xi, yi, 1.0f);
            prevxi = xi;
            prevyi = yi;
            
            phase += dphase;
        }
    } else {
        // set color to grey
        g.setColour(juce::Colour(118, 118, 118));
        
        Point<int> p1, p2;
        for (unsigned int i=0; i<points.size()-1; i++)
        {
            p1 = points[i];
            p2 = points[i+1];
            // we use drawAliasedLine because if we draw lines with AA we get nasty semi-transparent
            // blocks instead of solid color when the waveform is dense.
            drawAliasedLine(g, p1.x, p1.y, p2.x, p2.y);
        }
    }
    
    // BRAM: TODO clean this mess up...
    // ARMANDO: doesn't look that bad to me...
    
    // if we have a valid mouse location for displaying crosshairs and more info...
    if (where.x != -1) {
        
        g.setColour(juce::Colour(10, 10, 10));
        
        // draw crosshairs for mouse
        g.drawHorizontalLine(where.y, 0, size.getWidth() - 1);
        g.drawVerticalLine(where.x, 0, size.getHeight() - 1);
        
        // get x and y coordinates scaled for measurement purposes.
        float gain = powf(10.f, effect->getParameter(CSmartelectronixDisplay::kAmpWindow) * 6.f - 3.f);
        float y = (-2.f * ((float)where.y + 1.f) / (float)OSC_HEIGHT + 1.f) / gain;
        float x = (float)where.x * (float)counterSpeedInverse;
        char text[256];
        
        long lineSize = 10;
        
        g.setColour(juce::Colour(179, 111, 56));
        
        g.setFont(10.0f);
        
        // draw readout background
        g.drawImageAt(readout, 510, 2);
        
        // this is our sufficently sized rectangle
        Rectangle<int> textRect(512, 2, 105, 10 + lineSize);
        
        // print text to readout
        sprintf(text, "y = %.5f", y);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY()+lineSize);
        
        sprintf(text, "y = %.5f dB", cf_lin2db(fabsf(y)));
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY()+lineSize*2);
        
        sprintf(text, "x = %.2f samples", x);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY()+lineSize);
        
        sprintf(text, "x = %.5f seconds", x / sampleRate);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY()+lineSize);
        
        sprintf(text, "x = %.5f ms", 1000.f * x / sampleRate);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY()+lineSize);
        
        if (x == 0)
            sprintf(text, "x = infinite Hz");
        else
            sprintf(text, "x = %.3f Hz", sampleRate / x);
        
        g.drawText(text, textRect, kLeftText, true);
    }
    
}
