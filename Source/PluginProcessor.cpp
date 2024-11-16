/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SmexoscopeAudioProcessor::SmexoscopeAudioProcessor()
{
    smexoscopeProcessing = new CSmartelectronixDisplay();
}

SmexoscopeAudioProcessor::~SmexoscopeAudioProcessor()
{
}

//==============================================================================
const String SmexoscopeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SmexoscopeAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SmexoscopeAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

double SmexoscopeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SmexoscopeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SmexoscopeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SmexoscopeAudioProcessor::setCurrentProgram (int index)
{
}

const String SmexoscopeAudioProcessor::getProgramName (int index)
{
    return String();
}

void SmexoscopeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SmexoscopeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
    smexoscopeProcessing->setSampleRate(this->sampleRate);
}

void SmexoscopeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SmexoscopeAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin
    
    const int numChannels = preferredSet.size();
    
#if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
#elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
#else
    if (numChannels != 1 && numChannels != 2)
        return false;
    
    if (! setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
#endif
    
    return setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void SmexoscopeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Do audio processing...
    smexoscopeProcessing->processSub(&buffer,1);
}

//==============================================================================
bool SmexoscopeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SmexoscopeAudioProcessor::createEditor()
{
    return new SmexoscopeAudioProcessorEditor (*this);
}

//==============================================================================
void SmexoscopeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // save parameters
    destData.setSize(sizeof(float)*CSmartelectronixDisplay::kNumParams);
    destData.copyFrom(smexoscopeProcessing->getSaveBlock(), 0, sizeof(float)*CSmartelectronixDisplay::kNumParams);
}

void SmexoscopeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // recall parameters
    std::memcpy(smexoscopeProcessing->getSaveBlock(), data, sizeof(float)*CSmartelectronixDisplay::kNumParams);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SmexoscopeAudioProcessor();
}
