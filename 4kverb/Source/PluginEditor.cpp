// In PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor(_4kverbAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set custom LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Set knob colors to white
    setKnobColors(juce::Colours::black, juce::Colours::white, juce::Colours::black);

    // Add the following lines to the constructor
    menuBar = std::make_unique<juce::MenuBarComponent>(this);
    addAndMakeVisible(menuBar.get());

    // Add and attach new sliders
    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    predelaySlider.setRange(0.0, 500.0, 1.0); // Set interval to 1.0 for easier selection
    predelaySlider.setValue(20.0f); // Set default value
    predelaySlider.setLookAndFeel(&customLookAndFeel);
    predelaySlider.addListener(this); // Add listener
    predelaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    predelaySlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    predelaySlider.setTextValueSuffix(" ms"); // Add suffix
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    predelayLabel.setJustificationType(juce::Justification::centred);
    predelayLabel.setName("PreDelayLabel"); // Set the name
    predelayLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    predelayLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    mixSlider.setRange(0.0, 1.0, 0.01); // Set interval to 0.01 for easier selection
    mixSlider.setValue(1.0f); // Set default value
    mixSlider.setLookAndFeel(&customLookAndFeel);
    mixSlider.addListener(this); // Add listener
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    mixSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    mixSlider.setTextValueSuffix(" %"); // Add suffix
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setName("MixLabel"); // Set the name
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    mixLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    decaySlider.setRange(0.2, 70.0, 0.1); // Set interval to 0.1 for easier selection
    decaySlider.setValue(4.0f); // Set default value
    decaySlider.setLookAndFeel(&customLookAndFeel);
    decaySlider.addListener(this); // Add listener
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    decaySlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    decaySlider.setTextValueSuffix(" s"); // Add suffix
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setName("DecayLabel"); // Set the name
    decayLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    decayLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    sizeSlider.setRange(0.0, 1.0, 0.01); // Set interval to 0.01 for easier selection
    sizeSlider.setValue(0.5f); // Set default value
    sizeSlider.setLookAndFeel(&customLookAndFeel);
    sizeSlider.addListener(this); // Add listener
    sizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    sizeSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    sizeSlider.setTextValueSuffix(" %"); // Add suffix
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setJustificationType(juce::Justification::centred);
    sizeLabel.setName("SizeLabel"); // Set the name
    sizeLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    sizeLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    highCutSlider.setRange(1000.0, 21000.0, 100.0); // Set interval to 100.0 for easier selection
    highCutSlider.setValue(8000.0f); // Set default value
    highCutSlider.setLookAndFeel(&customLookAndFeel);
    highCutSlider.addListener(this); // Add listener
    highCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    highCutSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    highCutSlider.setName("HighCut"); // Set the name to identify the slider
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setName("HighCutLabel"); // Set the name
    highCutLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    highCutLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    lowCutSlider.setRange(10.0, 500.0, 10.0); // Set interval to 10.0 for easier selection
    lowCutSlider.setValue(10.0f); // Set default value
    lowCutSlider.setLookAndFeel(&customLookAndFeel);
    lowCutSlider.addListener(this); // Add listener
    lowCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    lowCutSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    lowCutSlider.setTextValueSuffix(" Hz"); // Add suffix
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setName("LowCutLabel"); // Set the name
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    lowCutLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(lowCutLabel);

    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    rateSlider.setRange(0.1, 10.0, 0.1); // Set interval to 0.1 for easier selection
    rateSlider.setValue(1.0f); // Set default value
    rateSlider.setLookAndFeel(&customLookAndFeel);
    rateSlider.addListener(this); // Add listener
    rateSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    rateSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    rateSlider.setTextValueSuffix(" Hz"); // Add suffix
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setName("RateLabel"); // Set the name
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    rateLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); // Adjust text box width and height
    depthSlider.setRange(0.0, 100.0, 1.0); // Set interval to 1.0 for easier selection
    depthSlider.setValue(50.0f); // Set default value
    depthSlider.setLookAndFeel(&customLookAndFeel);
    depthSlider.addListener(this); // Add listener
    depthSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); // Set text color to black
    depthSlider.setSliderSnapsToMousePosition(false); // Prevent jumping to click position
    depthSlider.setTextValueSuffix(" %"); // Add suffix
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.setJustificationType(juce::Justification::centred);
    depthLabel.setName("DepthLabel"); // Set the name
    depthLabel.setColour(juce::Label::textColourId, juce::Colours::black); // Set text color to black
    depthLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
    addAndMakeVisible(depthLabel);

    // Initialize background colors
    currentBackgroundColor = juce::Colours::white; // Starting background color
    targetBackgroundColor = currentBackgroundColor;
    colorTransitionSteps = 0;
    currentTransitionStep = 0;

    // Start the timer with an interval (e.g., 30 milliseconds)
    startTimer(30);

    setSize(200, 1000);
}

_4kverbAudioProcessorEditor::~_4kverbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

//==============================================================================

void _4kverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill the background with a color based on the slider values
    g.fillAll(getBackgroundColor());

    juce::ColourGradient gradient(juce::Colours::transparentWhite, 0, 0,
        juce::Colours::transparentWhite, 0, getHeight(), false);

    g.setGradientFill(gradient);
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colours::black);
    g.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); // Set font to Arial Black
}

void _4kverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 20;
    auto sliderDiameter = 100;
    auto labelWidth = 100; // Set a fixed width for the labels

    // Set the bounds for the menu bar
    menuBar->setBounds(bounds.removeFromTop(20));

    // Calculate the total height needed for all sliders and labels
    auto totalHeight = (sliderDiameter + labelHeight) * 8;

    // Calculate the starting Y position to center everything vertically
    auto startY = (bounds.getHeight() - totalHeight) / 2;

    auto area = bounds.removeFromTop(startY);

    auto centerX = bounds.getWidth() / 2;

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    predelayLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    predelaySlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    mixLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    mixSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    decayLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    decaySlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    sizeLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    sizeSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    highCutLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    highCutSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    lowCutLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    lowCutSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    rateLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    rateSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);

    area = bounds.removeFromTop(sliderDiameter + labelHeight);
    depthLabel.setBounds(centerX - labelWidth / 2, area.getY(), labelWidth, labelHeight);
    depthSlider.setBounds(centerX - sliderDiameter / 2, area.getY() + labelHeight, sliderDiameter, sliderDiameter);
}

void _4kverbAudioProcessorEditor::setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor)
{
    customLookAndFeel.setKnobColors(thumbColor, fillColor, outlineColor);
}

// In PluginEditor.cpp

void _4kverbAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // Calculate a target color based on the slider values
    float predelayValue = predelaySlider.getValue();
    float mixValue = mixSlider.getValue();
    float decayValue = decaySlider.getValue();
    float sizeValue = sizeSlider.getValue();
    float highCutValue = highCutSlider.getValue();
    float lowCutValue = lowCutSlider.getValue();
    float rateValue = rateSlider.getValue();
    float depthValue = depthSlider.getValue();

    // Combine the values to create a target color
    float red = (predelayValue / 500.0f + mixValue + decayValue / 70.0f) / 3.0f;
    float green = (sizeValue + highCutValue / 21000.0f + lowCutValue / 500.0f) / 3.0f;
    float blue = (rateValue / 10.0f + depthValue / 100.0f) / 2.0f;

    targetBackgroundColor = juce::Colour::fromFloatRGBA(red, green, blue, 1.0f);

    // Set up the transition
    colorTransitionSteps = 20; // Number of steps in the transition
    currentTransitionStep = 0;

    repaint(); // Repaint the background when a slider value changes
}

juce::Colour _4kverbAudioProcessorEditor::getBackgroundColor()
{
    // Calculate the background color based on the slider values
    float predelayValue = predelaySlider.getValue();
    float mixValue = mixSlider.getValue();
    float decayValue = decaySlider.getValue();
    float sizeValue = sizeSlider.getValue();
    float highCutValue = highCutSlider.getValue();
    float lowCutValue = lowCutSlider.getValue();
    float rateValue = rateSlider.getValue();
    float depthValue = depthSlider.getValue();

    // Combine the values to create a color
    float red = (predelayValue / 500.0f + mixValue + decayValue / 70.0f) / 3.0f;
    float green = (sizeValue + highCutValue / 21000.0f + lowCutValue / 500.0f) / 3.0f;
    float blue = (rateValue / 10.0f + depthValue / 100.0f) / 2.0f;

    return juce::Colour::fromFloatRGBA(red, green, blue, 1.0f);
}

void _4kverbAudioProcessorEditor::timerCallback()
{
    if (colorTransitionSteps > 0)
    {
        float alpha = (float)currentTransitionStep / (float)colorTransitionSteps;

        // Interpolate between the current and target colors
        juce::Colour newColor = currentBackgroundColor.interpolatedWith(targetBackgroundColor, alpha);

        // Update the current background color
        currentBackgroundColor = newColor;

        // Increment the transition step
        ++currentTransitionStep;

        // Repaint the editor
        repaint();

        // Check if the transition is complete
        if (currentTransitionStep >= colorTransitionSteps)
        {
            // Finalize the color
            currentBackgroundColor = targetBackgroundColor;
            colorTransitionSteps = 0;
            currentTransitionStep = 0;
        }
    }

    // Get the audio level from the processor
    float audioLevel = audioProcessor.getAudioLevel();

    // Map the audio level to a brightness value (0.0 to 1.0)
    float brightness = juce::jmap(audioLevel, 0.0f, 1.0f, 0.0f, 1.0f);

    // Update the background color based on the audio level
    currentBackgroundColor = juce::Colour::fromHSV(currentBackgroundColor.getHue(), currentBackgroundColor.getSaturation(), brightness, 1.0f);

    // Repaint the editor
    repaint();
}

// In PluginEditor.cpp
// In PluginEditor.cpp

// In PluginEditor.cpp

// In PluginEditor.cpp

void _4kverbAudioProcessorEditor::loadPreset()
{
    DBG("loadPreset called");
    fileChooser = std::make_unique<juce::FileChooser>("Select a preset to load", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.preset");

    fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                DBG("File selected: " + file.getFullPathName());
                juce::FileInputStream inputStream(file);
                if (inputStream.openedOk())
                {
                    DBG("File opened successfully");
                    juce::MemoryBlock data;
                    inputStream.readIntoMemoryBlock(data);
                    audioProcessor.setStateInformation(data.getData(), static_cast<int>(data.getSize()));
                    DBG("Preset loaded from file: " + file.getFullPathName());
                }
                else
                {
                    DBG("Failed to open file: " + file.getFullPathName());
                }
            }
            else
            {
                DBG("No file selected or file does not exist");
            }

            // Reset the fileChooser to allow future operations
            fileChooser.reset();
        });
}




void _4kverbAudioProcessorEditor::savePreset()
{
    DBG("savePreset called");
    fileChooser = std::make_unique<juce::FileChooser>("Save preset as...", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.preset");

    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{}) // Check if a file was selected
            {
                DBG("File selected: " + file.getFullPathName());
                juce::MemoryBlock data;
                audioProcessor.getStateInformation(data);
                auto result = file.replaceWithData(data.getData(), data.getSize());
                if (result)
                {
                    DBG("Preset saved to file: " + file.getFullPathName());
                }
                else
                {
                    DBG("Failed to save preset to file: " + file.getFullPathName());
                }
            }
            else
            {
                DBG("No file selected");
            }

            // Reset the fileChooser to allow future operations
            fileChooser.reset();
        });
}





juce::PopupMenu _4kverbAudioProcessorEditor::getMenuForIndex(int menuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;

    if (menuName == "File")
    {
        menu.addItem(_4kverbAudioProcessorEditor::MenuIDs::loadPresetID, "Load Preset");
        menu.addItem(_4kverbAudioProcessorEditor::MenuIDs::savePresetID, "Save Preset");
    }
    else if (menuName == "Presets")
    {
        menu.addItem(PresetIDs::preset1, "Preset 1");
        menu.addItem(PresetIDs::preset2, "Preset 2");
        menu.addItem(PresetIDs::preset3, "Preset 3");
    }

    return menu;
}

juce::StringArray _4kverbAudioProcessorEditor::getMenuBarNames()
{
    return { "File", "Presets" };
}

void _4kverbAudioProcessorEditor::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    switch (menuItemID)
    {
    case MenuIDs::loadPresetID:
        DBG("Load Preset menu item selected");
        loadPreset();
        break;
    case MenuIDs::savePresetID:
        DBG("Save Preset menu item selected");
        savePreset();
        break;
    case PresetIDs::preset1:
        // Apply settings for preset 1
        predelaySlider.setValue(50.0f);
        decaySlider.setValue(5.0f);
        sizeSlider.setValue(0.7f);
        highCutSlider.setValue(10000.0f);
        lowCutSlider.setValue(50.0f);
        rateSlider.setValue(2.0f);
        depthSlider.setValue(30.0f);
        break;
    case PresetIDs::preset2:
        // Apply settings for preset 2
        predelaySlider.setValue(100.0f);
        decaySlider.setValue(10.0f);
        sizeSlider.setValue(0.5f);
        highCutSlider.setValue(15000.0f);
        lowCutSlider.setValue(100.0f);
        rateSlider.setValue(3.0f);
        depthSlider.setValue(40.0f);
        break;
    case PresetIDs::preset3:
        // Apply settings for preset 3
        predelaySlider.setValue(200.0f);
        decaySlider.setValue(20.0f);
        sizeSlider.setValue(0.9f);
        highCutSlider.setValue(8000.0f);
        lowCutSlider.setValue(20.0f);
        rateSlider.setValue(1.0f);
        depthSlider.setValue(50.0f);
        break;
    default:
        break;
    }
}
