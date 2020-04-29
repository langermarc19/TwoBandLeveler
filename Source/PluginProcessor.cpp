/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TwoBandLeveler_3AudioProcessor::TwoBandLeveler_3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  ), tree(*this, nullptr,Identifier("Parameters"),
                          {
                              std::make_unique<AudioParameterFloat>("crossover", "crossover", 20.0, 20000.0, 2000.0),
                              std::make_unique<AudioParameterFloat>("band1Gain", "band1Gain", 0.0f, 2.0f, 1.0f),
                              std::make_unique<AudioParameterFloat>("band2Gain", "band2Gain", 0.0f, 2.0f, 1.0f)
                          })
#endif
{
    pCrossover = tree.getRawParameterValue("crossover");
    pBand1Gain = tree.getRawParameterValue("band1Gain");
    pBand2Gain = tree.getRawParameterValue("band2Gain");
}


TwoBandLeveler_3AudioProcessor::~TwoBandLeveler_3AudioProcessor()
{
}

//==============================================================================
const String TwoBandLeveler_3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TwoBandLeveler_3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TwoBandLeveler_3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TwoBandLeveler_3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TwoBandLeveler_3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TwoBandLeveler_3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TwoBandLeveler_3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TwoBandLeveler_3AudioProcessor::setCurrentProgram (int index)
{
}

const String TwoBandLeveler_3AudioProcessor::getProgramName (int index)
{
    return {};
}

void TwoBandLeveler_3AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TwoBandLeveler_3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampleRate = sampleRate;
    
    dsp::ProcessSpec spec;
    spec.sampleRate = mSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    updateProcessorChains();
    
    band1Chain.prepare(spec);
    band2Chain.prepare(spec);
}
void TwoBandLeveler_3AudioProcessor::updateProcessorChains()
{
    float crossover = *pCrossover;
    float band1gain = *pBand1Gain;
    float band2gain = *pBand2Gain;
    
    band1Chain.get<0>().state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    band1Chain.get<0>().state->setCutOffFrequency(mSampleRate, crossover, (1.0 / MathConstants<double>::sqrt2));
    band1Chain.get<1>().setGainLinear(band1gain);
    
    band2Chain.get<0>().state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    band2Chain.get<0>().state->setCutOffFrequency(mSampleRate, crossover, (1.0 / MathConstants<double>::sqrt2));
    band2Chain.get<1>().setGainLinear(band2gain);
}

void TwoBandLeveler_3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TwoBandLeveler_3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TwoBandLeveler_3AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto bufferLength = buffer.getNumSamples();
    
    for (auto i = totalNumInputChannels; i  < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //update processor chains with any new settings from parameters
    updateProcessorChains();
    
    //set up copy buffers to process each band seperately
    AudioBuffer<float> band1Buffer;
    AudioBuffer<float> band2Buffer;
    band1Buffer.setSize(totalNumInputChannels, bufferLength);
    band2Buffer.setSize(totalNumInputChannels, bufferLength);
    
    //copy input into each band's buffer
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* bufferData = buffer.getReadPointer (channel);
        
        band1Buffer.copyFrom(channel, 0, bufferData, bufferLength);
        band2Buffer.copyFrom(channel, 0, bufferData, bufferLength);
    }
    //apply the filter and gain processing to each band using dsp module features
    dsp::AudioBlock<float> band1Block (band1Buffer);
    dsp::AudioBlock<float> band2Block (band2Buffer);
    band1Chain.process(dsp::ProcessContextReplacing<float> (band1Block));
    band2Chain.process(dsp::ProcessContextReplacing<float> (band2Block));
    
    //clear input and sum the bands back into the output buffer
    buffer.clear();
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* band1Data = band1Buffer.getReadPointer(channel);
        auto* band2Data = band2Buffer.getReadPointer(channel);
        buffer.addFrom(channel, 0, band1Data, bufferLength);
        buffer.addFrom(channel, 0, band2Data, bufferLength);
    }
}

//==============================================================================
bool TwoBandLeveler_3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TwoBandLeveler_3AudioProcessor::createEditor()
{
    return new TwoBandLeveler_3AudioProcessorEditor (*this, tree);
}

//==============================================================================
void TwoBandLeveler_3AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TwoBandLeveler_3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TwoBandLeveler_3AudioProcessor();
}
