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
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    addAndMakeVisible(lowCutLabel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 800);
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
    auto sliderHeight = 30;

    predelayLabel.setBounds(bounds.removeFromTop(labelHeight));
    predelaySlider.setBounds(bounds.removeFromTop(sliderHeight));

    mixLabel.setBounds(bounds.removeFromTop(labelHeight));
    mixSlider.setBounds(bounds.removeFromTop(sliderHeight));

    decayLabel.setBounds(bounds.removeFromTop(labelHeight));
    decaySlider.setBounds(bounds.removeFromTop(sliderHeight));

    sizeLabel.setBounds(bounds.removeFromTop(labelHeight));
    sizeSlider.setBounds(bounds.removeFromTop(sliderHeight));

    highCutLabel.setBounds(bounds.removeFromTop(labelHeight));
    highCutSlider.setBounds(bounds.removeFromTop(sliderHeight));

    lowCutLabel.setBounds(bounds.removeFromTop(labelHeight));
    lowCutSlider.setBounds(bounds.removeFromTop(sliderHeight));

}
