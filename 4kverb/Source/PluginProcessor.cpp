#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_4kverbAudioProcessor::_4kverbAudioProcessor()
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output",  juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
      parameters(*this, nullptr, "PARAMETERS",
                 {
                     std::make_unique<juce::AudioParameterFloat>("predelay", "PreDelay", 0.0f, 1.0f, 0.0f),
                     std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("size", "Size", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("highCut", "High Cut", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("lowCut", "Low Cut", 0.0f, 1.0f, 0.5f)
                 })
{
    // Initialize reverb parameters
    reverbParams.roomSize = *parameters.getRawParameterValue("size");
    reverbParams.damping = *parameters.getRawParameterValue("decay");
    reverbParams.wetLevel = *parameters.getRawParameterValue("mix");
    reverbParams.dryLevel = 1.0f - reverbParams.wetLevel;
    reverbParams.width = 1.0f;
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
    return 1;
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
void _4kverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Set the sample rate for the reverb instance
    reverb.setSampleRate(sampleRate);

    // Set initial parameters
    reverb.setParameters(reverbParams);
}

void _4kverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _4kverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Only mono or stereo is supported
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    // Input and output layouts must match
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void _4kverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update reverb parameters if they have changed
    auto newRoomSize = parameters.getRawParameterValue("size")->load();
    auto newDamping = parameters.getRawParameterValue("decay")->load();
    auto newWetLevel = parameters.getRawParameterValue("mix")->load();
    auto newDryLevel = 1.0f - newWetLevel;

    if (reverbParams.roomSize != newRoomSize ||
        reverbParams.damping != newDamping ||
        reverbParams.wetLevel != newWetLevel ||
        reverbParams.dryLevel != newDryLevel)
    {
        reverbParams.roomSize = newRoomSize;
        reverbParams.damping = newDamping;
        reverbParams.wetLevel = newWetLevel;
        reverbParams.dryLevel = newDryLevel;
        reverbParams.width = 1.0f;

        reverb.setParameters(reverbParams);
    }

    // Create a wet buffer
    juce::AudioBuffer<float> wetBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);

    // Process the wet buffer with reverb
    if (buffer.getNumChannels() >= 2)
    {
        reverb.processStereo(wetBuffer.getWritePointer(0),
            wetBuffer.getWritePointer(1),
            buffer.getNumSamples());
    }
    else
    {
        reverb.processMono(wetBuffer.getWritePointer(0), buffer.getNumSamples());
    }

    // Mix dry and wet signals
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* dryData = buffer.getWritePointer(channel);
        auto* wetData = wetBuffer.getReadPointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            dryData[sample] = dryData[sample] * reverbParams.dryLevel + wetData[sample] * reverbParams.wetLevel;
        }
    }
}

//==============================================================================
bool _4kverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* _4kverbAudioProcessor::createEditor()
{
    return new _4kverbAudioProcessorEditor (*this);
}

//==============================================================================
void _4kverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void _4kverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml.get() != nullptr)
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorValueTreeState& _4kverbAudioProcessor::getParameters()
{
    return parameters;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _4kverbAudioProcessor();
}
