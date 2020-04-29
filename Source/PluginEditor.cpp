/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TwoBandLeveler_3AudioProcessorEditor::TwoBandLeveler_3AudioProcessorEditor (TwoBandLeveler_3AudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 500);
    
    band1GainLabel.setText("B1Gain", dontSendNotification);
    band1GainLabel.attachToComponent(&band1GainSlider, true);
    band1GainSliderAttachment.reset(new SliderAttachment (valueTreeState, "band1Gain", band1GainSlider));
    addAndMakeVisible(band1GainSlider);
    addAndMakeVisible(band1GainLabel);

    band2GainLabel.setText("B2Gain", dontSendNotification);
    band2GainLabel.attachToComponent(&band2GainSlider, true);
    band2GainSliderAttachment.reset(new SliderAttachment (valueTreeState, "band2Gain", band2GainSlider));
    addAndMakeVisible(band2GainSlider);
    addAndMakeVisible(band2GainLabel);
   
    crossover1Label.setText("Crossover", dontSendNotification);
    crossover1Label.attachToComponent(&crossover1Slider, true);
    crossover1SliderAttachment.reset(new SliderAttachment (valueTreeState, "crossover", crossover1Slider));
    addAndMakeVisible(crossover1Slider);
    addAndMakeVisible(crossover1Label);
}

TwoBandLeveler_3AudioProcessorEditor::~TwoBandLeveler_3AudioProcessorEditor()
{
}

//==============================================================================
void TwoBandLeveler_3AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.setFont (15.0f);

}

void TwoBandLeveler_3AudioProcessorEditor::resized()
{
    auto sliderLeft = 95;
    auto sliderWidth = (getWidth() - sliderLeft - 10);
    auto center = getWidth()/2;
    
    band1GainSlider.setBounds(sliderLeft, 200, sliderWidth, 20);
    band2GainSlider.setBounds(sliderLeft, 250, sliderWidth, 20);
   
    crossover1Slider.setBounds(sliderLeft , 300, sliderWidth, 20);
}

void TwoBandLeveler_3AudioProcessorEditor::parameterChanged(const String& parameterID, float newValue)
{}

