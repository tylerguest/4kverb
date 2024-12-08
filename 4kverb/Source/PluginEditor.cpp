/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================

*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor(_4kverbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Add and attach new sliders
    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    predelaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    highCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    addAndMakeVisible(lowCutLabel);

    // Add and attach modulation sliders
    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    depthSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    addAndMakeVisible(depthLabel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 1000);
}
_4kverbAudioProcessorEditor::~_4kverbAudioProcessorEditor()
{
}

//==============================================================================
void _4kverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::lightblue);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
}

void _4kverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 20;
    auto sliderDiameter = 100;

    auto area = bounds.removeFromTop(sliderDiameter + labelHeight);

    predelayLabel.setBounds(area.removeFromTop(labelHeight));
    predelayLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    predelaySlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    mixLabel.setBounds(area.removeFromTop(labelHeight));
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    mixSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    decayLabel.setBounds(area.removeFromTop(labelHeight));
    decayLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    decaySlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    sizeLabel.setBounds(area.removeFromTop(labelHeight));
    sizeLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    sizeSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    highCutLabel.setBounds(area.removeFromTop(labelHeight));
    highCutLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    highCutSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    lowCutLabel.setBounds(area.removeFromTop(labelHeight));
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    lowCutSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    rateLabel.setBounds(area.removeFromTop(labelHeight));
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    rateSlider.setBounds(area.removeFromTop(sliderDiameter));

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    depthLabel.setBounds(area.removeFromTop(labelHeight));
    depthLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set label text color to black
    depthSlider.setBounds(area.removeFromTop(sliderDiameter));
}
