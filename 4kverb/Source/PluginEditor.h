/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class _4kverbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _4kverbAudioProcessorEditor (_4kverbAudioProcessor&);
    ~_4kverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _4kverbAudioProcessor& audioProcessor;

    // Declare new sliders
    juce::Slider predelaySlider;
    juce::Slider mixSlider;
    juce::Slider decaySlider;
    juce::Slider sizeSlider;
    juce::Slider highCutSlider;
    juce::Slider lowCutSlider;

    // Declare new attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> predelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;

    // Declare new labels
    juce::Label predelayLabel;
    juce::Label mixLabel;
    juce::Label decayLabel;
    juce::Label sizeLabel;
    juce::Label highCutLabel;
    juce::Label lowCutLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_4kverbAudioProcessorEditor)
};
