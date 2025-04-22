#include "PluginProcessor.h"
#include "PluginEditor.h"

_4kverbAudioProcessor::_4kverbAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      parameters(*this, nullptr, "PARAMETERS",
                 {std::make_unique<juce::AudioParameterFloat>("predelay", "PreDelay", 0.0f, 500.0f, 20.0f),
                  std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 1.0f),
                  std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.2f, 70.0f, 4.0f),
                  std::make_unique<juce::AudioParameterFloat>("size", "Size", 0.0f, 1.0f, 0.5f),
                  std::make_unique<juce::AudioParameterFloat>("highCut", "High Cut", 1000.0f, 21000.0f, 8000.0f),
                  std::make_unique<juce::AudioParameterFloat>("lowCut", "Low Cut", 10.0f, 500.0f, 10.0f),
                  std::make_unique<juce::AudioParameterFloat>("rate", "Rate", 0.1f, 10.0f, 1.0f),
                  std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, 100.0f, 50.0f)})
{
    reverbParams.roomSize = *parameters.getRawParameterValue("size");
    reverbParams.damping = *parameters.getRawParameterValue("decay");
    reverbParams.wetLevel = *parameters.getRawParameterValue("mix");
    reverbParams.dryLevel = 1.0f - reverbParams.wetLevel;
    reverbParams.width = 1.0f;
}

_4kverbAudioProcessor::~_4kverbAudioProcessor()
{
}

juce::File _4kverbAudioProcessor::getDefaultPresetDirectory() const
{
    auto presetDirectory = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("4kverbPresets");
    if (!presetDirectory.exists())
        presetDirectory.createDirectory();
    return presetDirectory;
}

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

void _4kverbAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String _4kverbAudioProcessor::getProgramName(int index)
{
    return {};
}

void _4kverbAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

void _4kverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
    reverb.setParameters(reverbParams);

    auto totalNumChannels = getTotalNumOutputChannels();
    highCutFilters.clear();
    highCutFilters.resize(totalNumChannels);

    lowCutFilters.clear();
    lowCutFilters.resize(totalNumChannels);

    predelayLines.clear();
    predelayLines.resize(totalNumChannels); 

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1; 

    for (auto &delayLine : predelayLines)
    {
        delayLine.setMaximumDelayInSamples(static_cast<int>(sampleRate * 0.5));
        delayLine.prepare(spec);
        delayLine.reset();
    }

    for (auto &filter : highCutFilters)
    {
        filter.prepare(spec);
        filter.reset();
        filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 1000.0f);
    }

    for (auto &filter : lowCutFilters)
    {
        filter.prepare(spec);
        filter.reset();
        filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0f);
    }
}

void _4kverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _4kverbAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    return true;
#endif
}
#endif
void _4kverbAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto newRoomSize = parameters.getRawParameterValue("size")->load();
    auto newDamping = parameters.getRawParameterValue("decay")->load();
    auto newWetLevel = parameters.getRawParameterValue("mix")->load();
    auto newDryLevel = 1.0f - newWetLevel;
    auto newHighCut = parameters.getRawParameterValue("highCut")->load();
    auto newLowCut = parameters.getRawParameterValue("lowCut")->load();
    auto newPredelay = parameters.getRawParameterValue("predelay")->load();
    auto newRate = parameters.getRawParameterValue("rate")->load();
    auto newDepth = parameters.getRawParameterValue("depth")->load();
    auto newWidth = 0.8f;      
    auto newFreezeMode = 0.0f;

    if (reverbParams.roomSize != newRoomSize ||
        reverbParams.damping != newDamping ||
        reverbParams.wetLevel != newWetLevel ||
        reverbParams.dryLevel != newDryLevel ||
        reverbParams.width != newWidth ||
        reverbParams.freezeMode != newFreezeMode)
    {
        reverbParams.roomSize = newRoomSize;
        reverbParams.damping = newDamping;
        reverbParams.wetLevel = newWetLevel;
        reverbParams.dryLevel = newDryLevel;
        reverbParams.width = newWidth;
        reverbParams.freezeMode = newFreezeMode;

        reverb.setParameters(reverbParams);
    }

    juce::AudioBuffer<float> wetBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);

    for (int channel = 0; channel < wetBuffer.getNumChannels(); ++channel)
    {
        auto *wetData = wetBuffer.getWritePointer(channel);

        predelayLines[channel].setDelay(newPredelay * getSampleRate() / 1000.0f);

        for (int sample = 0; sample < wetBuffer.getNumSamples(); ++sample)
        {
            predelayLines[channel].pushSample(0, wetData[sample]);
            wetData[sample] = predelayLines[channel].popSample(0);
        }
    }

    static float phase = 0.0f;
    float phaseIncrement = 2.0f * juce::MathConstants<float>::pi * newRate / getSampleRate();
    for (int channel = 0; channel < wetBuffer.getNumChannels(); ++channel)
    {
        auto *wetData = wetBuffer.getWritePointer(channel);
        for (int sample = 0; sample < wetBuffer.getNumSamples(); ++sample)
        {
            float modulatedDelay = newPredelay + newDepth * std::sin(phase);
            modulatedDelay = juce::jlimit(0.0f, 500.0f, modulatedDelay); 
            predelayLines[channel].setDelay(modulatedDelay * getSampleRate() / 1000.0f);
            phase += phaseIncrement;
            if (phase >= 2.0f * juce::MathConstants<float>::pi)
                phase -= 2.0f * juce::MathConstants<float>::pi;
        }
    }

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

    juce::dsp::AudioBlock<float> wetBlock(wetBuffer);

    for (size_t channel = 0; channel < wetBlock.getNumChannels(); ++channel)
    {
        auto channelBlock = wetBlock.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        highCutFilters[channel].process(context);
    }

    for (size_t channel = 0; channel < wetBlock.getNumChannels(); ++channel)
    {
        auto channelBlock = wetBlock.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        lowCutFilters[channel].process(context);
    }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto *dryData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            dryData[sample] = dryData[sample] * reverbParams.dryLevel + wetData[sample] * reverbParams.wetLevel;
    }

    float currentLevel = 0.0f;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        currentLevel += buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
    }
    currentLevel /= static_cast<float>(buffer.getNumChannels());

    audioLevel.store(currentLevel);

    DBG("Audio Level: " << currentLevel);
}

bool _4kverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor *_4kverbAudioProcessor::createEditor()
{
    return new _4kverbAudioProcessorEditor(*this);
}

void _4kverbAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    DBG("getStateInformation called");
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
    DBG("State information copied to memory block");
}

void _4kverbAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    DBG("setStateInformation called");
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr)
    {
        DBG("XML data loaded successfully");
        if (xml->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
            DBG("State information replaced successfully");
        }
        else
        {
            DBG("XML does not have the correct tag name");
        }
    }
    else
    {
        DBG("Failed to load XML from binary data");
    }
}

juce::AudioProcessorValueTreeState &_4kverbAudioProcessor::getParameters()
{
    return parameters;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new _4kverbAudioProcessor();
}
