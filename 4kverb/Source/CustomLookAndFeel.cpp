#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors or other initializations if needed
}

// CustomLookAndFeel.cpp

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
    g.setColour(fillColor); // Ensure transparency is applied
    g.fillEllipse(rx, ry, rw, rw);

    // Outline
    g.setColour(outlineColor); // Ensure transparency is applied
    g.drawEllipse(rx, ry, rw, rw, 5.0f); // Set outline thickness to 2.0f

    // Thumb (notch) as a line
    auto thumbWidth = 6.0f;
    auto thumbLength = radius / 2.0f; // Set thumb length to half the radius

    // Calculate the thumb positions
    auto thumbX1 = centerX + (radius - 2.0f) * std::cos(angle); // Position at the border
    auto thumbY1 = centerY + (radius - 2.0f) * std::sin(angle); // Position at the border
    auto thumbX2 = centerX + (radius - 2.0f - thumbLength) * std::cos(angle); // Half the length
    auto thumbY2 = centerY + (radius - 2.0f - thumbLength) * std::sin(angle); // Half the length

    g.setColour(thumbColor); // Ensure transparency is applied
    g.drawLine(thumbX1, thumbY1, thumbX2, thumbY2, thumbWidth);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(label.findColour(juce::Label::textColourId));

    // Set the font to a more readable bold font
    juce::Font font(juce::Font::getDefaultSansSerifFontName(), label.getFont().getHeight(), juce::Font::bold);
    g.setFont(font);

    // Convert the label text to uppercase
    juce::String text = label.getText().toUpperCase();

    auto textArea = label.getLocalBounds();
    g.drawFittedText(text, textArea, juce::Justification::centred, 1);
}
