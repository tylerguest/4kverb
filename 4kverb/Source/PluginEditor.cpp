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
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(dryLevelSlider);
    addAndMakeVisible(widthSlider);
    addAndMakeVisible(freezeModeButton);

    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "roomSize", roomSizeSlider);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "damping", dampingSlider);
    wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "wetLevel", wetLevelSlider);
    dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "dryLevel", dryLevelSlider);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "width", widthSlider);
    freezeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getParameters(), "freezeMode", freezeModeButton);


    // Initialize and add labels
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    widthLabel.setText("Width", juce::dontSendNotification);
    freezeModeLabel.setText("Freeze Mode", juce::dontSendNotification);

    addAndMakeVisible(roomSizeLabel);
    addAndMakeVisible(dampingLabel);
    addAndMakeVisible(wetLevelLabel);
    addAndMakeVisible(dryLevelLabel);
    addAndMakeVisible(widthLabel);
    addAndMakeVisible(freezeModeLabel);

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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    auto labelHeight = 20;
    auto sliderHeight = 50;

    roomSizeLabel.setBounds(area.removeFromTop(labelHeight));
    roomSizeSlider.setBounds(area.removeFromTop(sliderHeight));
    dampingLabel.setBounds(area.removeFromTop(labelHeight));
    dampingSlider.setBounds(area.removeFromTop(sliderHeight));
    wetLevelLabel.setBounds(area.removeFromTop(labelHeight));
    wetLevelSlider.setBounds(area.removeFromTop(sliderHeight));
    dryLevelLabel.setBounds(area.removeFromTop(labelHeight));
    dryLevelSlider.setBounds(area.removeFromTop(sliderHeight));
    widthLabel.setBounds(area.removeFromTop(labelHeight));
    widthSlider.setBounds(area.removeFromTop(sliderHeight));
    freezeModeLabel.setBounds(area.removeFromTop(labelHeight));
    freezeModeButton.setBounds(area.removeFromTop(sliderHeight));

}
