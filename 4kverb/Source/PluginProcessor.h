#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
*/
class _4kverbAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    _4kverbAudioProcessor();
    ~_4kverbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #if ! JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Provide a getter method for parameters
    juce::AudioProcessorValueTreeState& getParameters();

    float getAudioLevel() const { return audioLevel.load(); }
    juce::File getDefaultPresetDirectory() const;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    juce::dsp::DelayLine<float> preDelayLine;

    juce::Reverb reverb;

    juce::Reverb::Parameters reverbParams;

    // Declare vectors of filters for handling multiple channels
    std::vector<juce::dsp::IIR::Filter<float>> highCutFilters;
    std::vector<juce::dsp::IIR::Filter<float>> lowCutFilters;

    float lastHighCut = -1.0f; // Store the last highCut value to detect changes
    float lastLowCut = -1.0f;  // Store the last lowCut value to detect changes

    // In your plugin processor class
    std::vector<juce::dsp::DelayLine<float>> predelayLines;

    std::atomic<float> audioLevel{ 0.0f };


    float modulationDepth = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_4kverbAudioProcessor)
};
