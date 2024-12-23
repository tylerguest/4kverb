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

    // Add this method declaration
    juce::Font getMenuBarFont(juce::MenuBarComponent& menuBar, int itemIndex, const juce::String& itemText) override;

    // Add this method declaration
    float CustomLookAndFeel::calculateMenuBarFontSize(const juce::MenuBarComponent& menuBar, juce::MenuBarModel& menuBarModel);

private:
    juce::Colour thumbColor;
    juce::Colour fillColor;
    juce::Colour outlineColor;

    std::unique_ptr<juce::Typeface> robotoTypeface;
};

class CustomSlider : public juce::Slider
{
public:
    CustomSlider() = default;

    juce::String getTextFromValue(double value) override
    {
        if (getName() == "HighCut")
            return juce::String(value / 1000.0, 1) + " kHz";
        return juce::Slider::getTextFromValue(value);
    }
};
