/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================

*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor (_4kverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Add and attach new sliders
    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    addAndMakeVisible(lowCutLabel);

    // Add and attach modulation sliders
    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    addAndMakeVisible(depthLabel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 1000);
}
_4kverbAudioProcessorEditor::~_4kverbAudioProcessorEditor()
{
}

//==============================================================================
void _4kverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void _4kverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 20;
    auto sliderDiameter = 100;

    auto area = bounds.removeFromTop(sliderDiameter + labelHeight);

    predelayLabel.setBounds(area.removeFromTop(labelHeight));
    predelaySlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    mixLabel.setBounds(area.removeFromTop(labelHeight));
    mixSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    decayLabel.setBounds(area.removeFromTop(labelHeight));
    decaySlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    sizeLabel.setBounds(area.removeFromTop(labelHeight));
    sizeSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    highCutLabel.setBounds(area.removeFromTop(labelHeight));
    highCutSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    lowCutLabel.setBounds(area.removeFromTop(labelHeight));
    lowCutSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    rateLabel.setBounds(area.removeFromTop(labelHeight));
    rateSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    depthLabel.setBounds(area.removeFromTop(labelHeight));
    depthSlider.setBounds(area.removeFromTop(sliderDiameter));

}
