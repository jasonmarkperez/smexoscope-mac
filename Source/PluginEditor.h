/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "WaveDisplay.h"
#include "CustomKnob.h"
#include "TextElement.h"
#include "CustomSlider.h"
#include "JuceMultiStateButton.h"

//==============================================================================
/**
 */
class SmexoscopeAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SmexoscopeAudioProcessorEditor (SmexoscopeAudioProcessor&);
    ~SmexoscopeAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void updateParameters();
    
    double sampleRate = 44100;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SmexoscopeAudioProcessor& processor;
    juce::Image background;
    juce::Image heads;
    juce::Image readout;
    juce::Image tealKnob;
    juce::Image retrigMode;
    juce::Image leftRight;
    juce::Image onOff;
    juce::Image sliderNew;
    
    CustomKnob* timeKnob;
    CustomKnob* ampKnob;
    CustomKnob* intTrigSpeedKnob;
    CustomKnob* retrigThreshKnob;
    
    CustomSlider* retrigLevelSlider;
    
    JuceMultiStateButton* retriggerModeButton;
    JuceMultiStateButton* syncRedrawButton;
    JuceMultiStateButton* freezeButton;
    JuceMultiStateButton* dcKillButton;
    JuceMultiStateButton* channelSelectionButton;
    
    TextElement* timeText;
    TextElement* ampText;
    TextElement* speedText;
    TextElement* threshText;
    
    Timer* timepoint;
    
    CWaveDisplay* smexoscopeDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmexoscopeAudioProcessorEditor)
};

// special timer used to update gui on a clock
class ExoscopeRedrawTimer : public Timer
{
public:
    ExoscopeRedrawTimer(CWaveDisplay* target, SmexoscopeAudioProcessorEditor* editor)
    {
        smexoscopeDisplay = target;
        this->editor = editor;
    }
    void timerCallback() override {
        smexoscopeDisplay->repaint();
        editor->updateParameters();
    }
private:
    CWaveDisplay* smexoscopeDisplay;
    SmexoscopeAudioProcessorEditor* editor;
};
#endif  // PLUGINEDITOR_H_INCLUDED
