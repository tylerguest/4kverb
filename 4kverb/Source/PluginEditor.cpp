#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor(_4kverbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set custom LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Add and attach new sliders
    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    addAndMakeVisible(lowCutLabel);

    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
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
