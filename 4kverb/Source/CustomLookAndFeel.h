#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    void setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor);

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

private:
    juce::Colour thumbColor;
    juce::Colour fillColor;
    juce::Colour outlineColor;
};
