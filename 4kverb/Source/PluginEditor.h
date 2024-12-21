#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class _4kverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    _4kverbAudioProcessorEditor(_4kverbAudioProcessor&);
    ~_4kverbAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor);

private:
    _4kverbAudioProcessor& audioProcessor;

    CustomLookAndFeel customLookAndFeel;

    juce::Slider predelaySlider;
    juce::Label predelayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> predelayAttachment;

    juce::Slider mixSlider;
    juce::Label mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    juce::Slider decaySlider;
    juce::Label decayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;

    juce::Slider sizeSlider;
    juce::Label sizeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;

    juce::Slider highCutSlider;
    juce::Label highCutLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;

    juce::Slider lowCutSlider;
    juce::Label lowCutLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;

    juce::Slider rateSlider;
    juce::Label rateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;

    juce::Slider depthSlider;
    juce::Label depthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(_4kverbAudioProcessorEditor)
};
