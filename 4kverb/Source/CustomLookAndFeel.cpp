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
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centerX = (float)x + (float)width * 0.5f;
    auto centerY = (float)y + (float)height * 0.5f;
    auto rx = centerX - radius;
    auto ry = centerY - radius;
    auto rw = radius * 2.0f;

    // Correct the angle calculation
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle) - juce::MathConstants<float>::halfPi;

    // Drop shadow
    juce::DropShadow shadow(juce::Colours::black.withAlpha(0.5f), 5, juce::Point<int>(2, 2));
    juce::Path shadowPath;
    shadowPath.addEllipse(rx, ry, rw, rw);
    shadow.drawForPath(g, shadowPath);

    // Fill
    g.setColour(fillColor);
    g.fillEllipse(rx, ry, rw, rw);

    // Outline
    g.setColour(outlineColor);
    g.drawEllipse(rx, ry, rw, rw, 2.0f); // Set outline thickness to 2.0f

    // Thumb (notch) as a line
    auto thumbWidth = 6.0f; // Increased thumb width
    auto thumbLength = radius * 0.7f; // Set thumb length to 70% of the main radius

    // Calculate the thumb positions
    auto thumbX1 = centerX + thumbLength * std::cos(angle);
    auto thumbY1 = centerY + thumbLength * std::sin(angle);
    auto thumbX2 = centerX;
    auto thumbY2 = centerY;

    g.setColour(thumbColor); // Set color to thumbColor
    g.drawLine(thumbX1, thumbY1, thumbX2, thumbY2, thumbWidth);
}


void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(label.findColour(juce::Label::textColourId));

    // Set the font to a more readable bold font
    juce::Font font("Arial Black", label.getFont().getHeight(), juce::Font::bold);
    g.setFont(font);

    // Convert the label text to uppercase
    juce::String text = label.getText().toUpperCase();

    auto textArea = label.getLocalBounds();
    g.drawFittedText(text, textArea, juce::Justification::centred, 1);
}
