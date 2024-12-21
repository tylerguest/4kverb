#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightblue);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    }

    void setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor)
    {
        setColour(juce::Slider::thumbColourId, thumbColor);
        setColour(juce::Slider::rotarySliderFillColourId, fillColor);
        setColour(juce::Slider::rotarySliderOutlineColourId, outlineColor);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f;
        auto centerX = (float) x + (float) width  * 0.5f;
        auto centerY = (float) y + (float) height * 0.5f;
        auto rx = centerX - radius;
        auto ry = centerY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Fill
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.fillEllipse(rx, ry, rw, rw);

        // Outline
        g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        // Draw the notch
        juce::Path p;
        auto pointerLength = radius * 0.6f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));

        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        g.fillPath(p);
    }
};
