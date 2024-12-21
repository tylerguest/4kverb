#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor(_4kverbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set custom LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Set knob colors to a lighter blue than the background
    setKnobColors(juce::Colours::white, juce::Colours::lightblue, juce::Colours::black);

    // Add and attach new sliders
    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    predelayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lowCutLabel);

    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(depthLabel);

    setSize(400, 1000);
}

_4kverbAudioProcessorEditor::~_4kverbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void _4kverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightblue);
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
}

void _4kverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 20;
    auto sliderDiameter = 100;
    auto labelWidth = 100; // Set a fixed width for the labels

    // Calculate the total height needed for all sliders and labels
    auto totalHeight = (sliderDiameter + labelHeight) * 8;

    // Calculate the starting Y position to center everything vertically
    auto startY = (bounds.getHeight() - totalHeight) / 2;

    auto area = bounds.removeFromTop(startY);

    auto centerX = bounds.getWidth() / 2;

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    predelayLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    predelaySlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    mixLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    mixSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    decayLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    decaySlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    sizeLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    sizeSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    highCutLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    highCutSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    lowCutLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    lowCutSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    rateLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    rateSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    depthLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    depthSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);
}




void _4kverbAudioProcessorEditor::setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor)
{
    customLookAndFeel.setKnobColors(thumbColor, fillColor, outlineColor);
}
