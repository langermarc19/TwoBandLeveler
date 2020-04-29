/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TwoBandLeveler_3AudioProcessorEditor  : public AudioProcessorEditor,
                                              private AudioProcessorValueTreeState::Listener
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    TwoBandLeveler_3AudioProcessorEditor (TwoBandLeveler_3AudioProcessor&, AudioProcessorValueTreeState&);
    ~TwoBandLeveler_3AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void parameterChanged(const String&, float) override;
    
    AudioProcessorValueTreeState& valueTreeState;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TwoBandLeveler_3AudioProcessor& processor;
    
    //gain GUI
    Label band1GainLabel;
    Slider band1GainSlider;
    std::unique_ptr<SliderAttachment> band1GainSliderAttachment;
    
    Label band2GainLabel;
    Slider band2GainSlider;
    std::unique_ptr<SliderAttachment> band2GainSliderAttachment;
    
    //crossover GUI
    Label crossover1Label;
    Slider crossover1Slider;
    std::unique_ptr<SliderAttachment> crossover1SliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoBandLeveler_3AudioProcessorEditor)
};
