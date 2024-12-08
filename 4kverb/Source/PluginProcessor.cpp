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
                     std::make_unique<juce::AudioParameterFloat>("predelay", "PreDelay", 0.0f, 500.0f, 20.0f),
                     std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 1.0f),
                     std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.2f, 70.0f, 4.0f),
                     std::make_unique<juce::AudioParameterFloat>("size", "Size", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("highCut", "High Cut", 1000.0f, 21000.0f, 8000.0f),
                     std::make_unique<juce::AudioParameterFloat>("lowCut", "Low Cut", 10.0f, 500.0f, 10.0f)
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
    reverb.setSampleRate(sampleRate);

    // Prepare reverb with initial parameters
    reverb.setParameters(reverbParams);

    // Prepare high cut filters for each channel
    auto totalNumChannels = getTotalNumOutputChannels();
    highCutFilters.clear();
    highCutFilters.resize(totalNumChannels);

    lowCutFilters.clear();
    lowCutFilters.resize(totalNumChannels);

    predelayLines.clear();
    predelayLines.resize(totalNumChannels); // Ensure predelayLines is resized

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1; // Each filter handles one channel

    for (auto& delayLine : predelayLines)
    {
        delayLine.setMaximumDelayInSamples(static_cast<int>(sampleRate * 0.5)); // 500ms max predelay
        delayLine.prepare(spec);
        delayLine.reset();
    }

    for (auto& filter : highCutFilters)
    {
        filter.prepare(spec);
        filter.reset();

        // Initialize with default low-pass coefficients (optional)
        filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 1000.0f);
    }

    for (auto& filter : lowCutFilters)
    {
        filter.prepare(spec);
        filter.reset();

        // Initialize with default high-pass coefficients (optional)
        filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0f);
    }

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

    // Retrieve parameter values
    auto newRoomSize = parameters.getRawParameterValue("size")->load();
    auto newDamping = parameters.getRawParameterValue("decay")->load();
    auto newWetLevel = parameters.getRawParameterValue("mix")->load();
    auto newDryLevel = 1.0f - newWetLevel;
    auto newHighCut = parameters.getRawParameterValue("highCut")->load();
    auto newLowCut = parameters.getRawParameterValue("lowCut")->load();
    auto newPredelay = parameters.getRawParameterValue("predelay")->load();

    // Update reverb parameters if they've changed
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

    // Update high cut filter coefficients if `highCut` parameter has changed
    if (lastHighCut != newHighCut)
    {
        auto highCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), newHighCut);

        for (auto& filter : highCutFilters)
        {
            *filter.coefficients = *highCutCoefficients;
        }

        lastHighCut = newHighCut;
    }

    // Update low cut filter coefficients if `lowCut` parameter has changed
    if (lastLowCut != newLowCut)
    {
        auto lowCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), newLowCut);

        for (auto& filter : lowCutFilters)
        {
            *filter.coefficients = *lowCutCoefficients;
        }

        lastLowCut = newLowCut;
    }

    // Create a wet buffer and copy the input buffer into it
    juce::AudioBuffer<float> wetBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);

    // Apply predelay to the wet buffer
    for (int channel = 0; channel < wetBuffer.getNumChannels(); ++channel)
    {
        auto* wetData = wetBuffer.getWritePointer(channel);

        // Update predelay time for each channel
        predelayLines[channel].setDelay(newPredelay * getSampleRate() / 1000.0f);

        for (int sample = 0; sample < wetBuffer.getNumSamples(); ++sample)
        {
            predelayLines[channel].pushSample(0, wetData[sample]);
            wetData[sample] = predelayLines[channel].popSample(0);
        }
    }

    // Process the wet buffer with reverb
    if (wetBuffer.getNumChannels() >= 2)
    {
        reverb.processStereo(wetBuffer.getWritePointer(0),
            wetBuffer.getWritePointer(1),
            wetBuffer.getNumSamples());
    }
    else if (wetBuffer.getNumChannels() == 1)
    {
        reverb.processMono(wetBuffer.getWritePointer(0), wetBuffer.getNumSamples());
    }

    // Apply high cut filter to each channel of the wet buffer
    juce::dsp::AudioBlock<float> wetBlock(wetBuffer);

    for (size_t channel = 0; channel < wetBlock.getNumChannels(); ++channel)
    {
        auto channelBlock = wetBlock.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        highCutFilters[channel].process(context);
    }

    // Apply low cut filter to each channel of the wet buffer
    for (size_t channel = 0; channel < wetBlock.getNumChannels(); ++channel)
    {
        auto channelBlock = wetBlock.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        lowCutFilters[channel].process(context);
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
