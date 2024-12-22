#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors or other initializations if needed
}

void CustomLookAndFeel::setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor)
{
    this->thumbColor = thumbColor;
    this->fillColor = fillColor;
    this->outlineColor = outlineColor;
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // Custom drawing code for rotary sliders
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centerX = (float)x + (float)width * 0.5f;
    auto centerY = (float)y + (float)height * 0.5f;
    auto rx = centerX - radius;
    auto ry = centerY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Fill
    g.setColour(fillColor);
    g.fillEllipse(rx, ry, rw, rw);

    // Outline
    g.setColour(outlineColor);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));

    // Thumb
    g.setColour(thumbColor);
    g.fillPath(p);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(label.findColour(juce::Label::textColourId));

    // Check if the label is a descriptive label for a slider
    if (label.getName().contains("Label"))
    {
        // Set the font to bold for slider descriptive labels
        g.setFont(label.getFont().boldened());
    }
    else
    {
        g.setFont(label.getFont());
    }

    auto textArea = label.getLocalBounds();
    g.drawFittedText(label.getText(), textArea, juce::Justification::centred, 1);
}
