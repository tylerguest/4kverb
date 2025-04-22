#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
}

void CustomLookAndFeel::setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor)
{
    this->thumbColor = thumbColor;
    this->fillColor = fillColor;
    this->outlineColor = outlineColor;
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                         float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centerX = (float)x + (float)width * 0.5f;
    auto centerY = (float)y + (float)height * 0.5f;
    auto rx = centerX - radius;
    auto ry = centerY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle) - juce::MathConstants<float>::halfPi;


    juce::DropShadow shadow(juce::Colours::black.withAlpha(0.5f), 5, juce::Point<int>(2, 2));
    juce::Path shadowPath;
    shadowPath.addEllipse(rx, ry, rw, rw);
    shadow.drawForPath(g, shadowPath);

    g.setColour(fillColor); 
    g.fillEllipse(rx, ry, rw, rw);
    g.setColour(outlineColor);          
    g.drawEllipse(rx, ry, rw, rw, 5.0f);
  
    auto thumbWidth = 6.0f;
    auto thumbLength = radius / 2.0f; 
    auto thumbX1 = centerX + (radius - 2.0f) * std::cos(angle);                  // Position at the border
    auto thumbY1 = centerY + (radius - 2.0f) * std::sin(angle);                  // Position at the border
    auto thumbX2 = centerX + (radius - 2.0f - thumbLength) * std::cos(angle);    // Half the length
    auto thumbY2 = centerY + (radius - 2.0f - thumbLength) * std::sin(angle);    // Half the length

    g.setColour(thumbColor); 
    g.drawLine(thumbX1, thumbY1, thumbX2, thumbY2, thumbWidth);
}

void CustomLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label)
{
    g.setColour(label.findColour(juce::Label::textColourId));
    juce::Font font(juce::Font::getDefaultSansSerifFontName(), label.getFont().getHeight(), juce::Font::bold);
    g.setFont(font);

    juce::String text = label.getText().toUpperCase();

    auto textArea = label.getLocalBounds();
    g.drawFittedText(text, textArea, juce::Justification::centred, 1);
}

juce::Font CustomLookAndFeel::getMenuBarFont(juce::MenuBarComponent &menuBar, int itemIndex, const juce::String &itemText)
{
    if (auto *menuBarModel = menuBar.getModel())
    {
        float fontSize = calculateMenuBarFontSize(menuBar, *menuBarModel);
        return juce::Font(juce::Font::getDefaultSansSerifFontName(), fontSize, juce::Font::plain);
    }
    return juce::Font(juce::Font::getDefaultSansSerifFontName(), 12.0f, juce::Font::plain);
}

float CustomLookAndFeel::calculateMenuBarFontSize(const juce::MenuBarComponent &menuBar, juce::MenuBarModel &menuBarModel)
{
    auto menuBarNames = menuBarModel.getMenuBarNames();
    auto totalAvailableWidth = static_cast<float>(menuBar.getWidth()) - (menuBarNames.size() * 20.0f);
    float fontSize = 16.0f;                                                                           
    juce::Font font(juce::Font::getDefaultSansSerifFontName(), fontSize, juce::Font::plain);

    float totalRequiredWidth = 0.0f;

    for (const auto &menuName : menuBarNames)
        totalRequiredWidth += font.getStringWidth(menuName) + 10.0f;

    while (totalRequiredWidth > totalAvailableWidth && fontSize > 10.0f)
    {
        fontSize -= 0.5f;
        font.setHeight(fontSize);
        totalRequiredWidth = 0.0f;

        for (const auto &menuName : menuBarNames)
            totalRequiredWidth += font.getStringWidth(menuName) + 20.0f;

    return fontSize;
}
