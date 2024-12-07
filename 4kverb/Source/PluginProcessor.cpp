/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================

*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_4kverbAudioProcessor::_4kverbAudioProcessor()
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
      parameters(*this, nullptr, "PARAMETERS",
                 {
                     std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("wetLevel", "Wet Level", 0.0f, 1.0f, 0.33f),
                     std::make_unique<juce::AudioParameterFloat>("dryLevel", "Dry Level", 0.0f, 1.0f, 0.4f),
                     std::make_unique<juce::AudioParameterFloat>("width", "Width", 0.0f, 1.0f, 1.0f),
                     std::make_unique<juce::AudioParameterBool>("freezeMode", "Freeze Mode", false)
                 })
{
    reverbParams.roomSize = *parameters.getRawParameterValue("roomSize");
    reverbParams.damping = *parameters.getRawParameterValue("damping");
    reverbParams.wetLevel = *parameters.getRawParameterValue("wetLevel");
    reverbParams.dryLevel = *parameters.getRawParameterValue("dryLevel");
    reverbParams.width = *parameters.getRawParameterValue("width");
    reverbParams.freezeMode = *parameters.getRawParameterValue("freezeMode");
    reverb.setParameters(reverbParams);
}

_4kverbAudioProcessor::~_4kverbAudioProcessor()
{
}

//==============================================================================
const juce::String _4kverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _4kverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _4kverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _4kverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _4kverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _4kverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _4kverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _4kverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _4kverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void _4kverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _4kverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
}

void _4kverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _4kverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void _4kverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update reverb parameters
    reverbParams.roomSize = *parameters.getRawParameterValue("roomSize");
    reverbParams.damping = *parameters.getRawParameterValue("damping");
    reverbParams.wetLevel = *parameters.getRawParameterValue("wetLevel");
    reverbParams.dryLevel = *parameters.getRawParameterValue("dryLevel");
    reverbParams.width = *parameters.getRawParameterValue("width");
    reverbParams.freezeMode = *parameters.getRawParameterValue("freezeMode");
    reverb.setParameters(reverbParams);

    // Apply reverb to each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        reverb.processMono(channelData, buffer.getNumSamples());
    }
}

//==============================================================================
bool _4kverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _4kverbAudioProcessor::createEditor()
{
    return new _4kverbAudioProcessorEditor (*this);
}

//==============================================================================
void _4kverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(*parameters.getRawParameterValue("roomSize"));
    stream.writeFloat(*parameters.getRawParameterValue("damping"));
    stream.writeFloat(*parameters.getRawParameterValue("wetLevel"));
    stream.writeFloat(*parameters.getRawParameterValue("dryLevel"));
    stream.writeFloat(*parameters.getRawParameterValue("width"));
    stream.writeBool(*parameters.getRawParameterValue("freezeMode"));
}

void _4kverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);
    parameters.getParameter("roomSize")->setValueNotifyingHost(stream.readFloat());
    parameters.getParameter("damping")->setValueNotifyingHost(stream.readFloat());
    parameters.getParameter("wetLevel")->setValueNotifyingHost(stream.readFloat());
    parameters.getParameter("dryLevel")->setValueNotifyingHost(stream.readFloat());
    parameters.getParameter("width")->setValueNotifyingHost(stream.readFloat());
    parameters.getParameter("freezeMode")->setValueNotifyingHost(stream.readBool());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _4kverbAudioProcessor();
}
