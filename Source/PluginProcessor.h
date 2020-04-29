/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 */
class TwoBandLeveler_3AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TwoBandLeveler_3AudioProcessor();
    ~TwoBandLeveler_3AudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //===========Functions=======================
    void updateProcessorChains();
    //===========Public Member Variables==========
    AudioProcessorValueTreeState tree;
    double band1Gain, band2Gain;
    //============Parameters=====================
    std::atomic<float>* pCrossover;
    std::atomic<float>* pBand1Gain;
    std::atomic<float>* pBand2Gain;
    //===========DSP Processing=============
    dsp::ProcessorChain<dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>>, dsp::Gain<float>> band1Chain;
    dsp::ProcessorChain<dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>>, dsp::Gain<float>> band2Chain;
    
    
private:
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoBandLeveler_3AudioProcessor)
};
