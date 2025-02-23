#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
 */
class _4kverbAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::Slider::Listener,
                                    public juce::Timer,
                                    public juce::MenuBarModel
{
public:
    _4kverbAudioProcessorEditor(_4kverbAudioProcessor &);
    ~_4kverbAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void setKnobColors(juce::Colour thumbColor, juce::Colour fillColor, juce::Colour outlineColor);

    // Implement the sliderValueChanged method
    void sliderValueChanged(juce::Slider *slider) override;

    // Override timerCallback
    void timerCallback() override; // Add this

    // With this line:
    juce::StringArray getMenuBarNames() override;

    juce::PopupMenu getMenuForIndex(int menuIndex, const juce::String &menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
    _4kverbAudioProcessor &audioProcessor;

    CustomLookAndFeel customLookAndFeel;

    juce::Slider predelaySlider;
    juce::Label predelayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> predelayAttachment;

    juce::Slider mixSlider;
    juce::Label mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    juce::Slider decaySlider;
    juce::Label decayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;

    juce::Slider sizeSlider;
    juce::Label sizeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttachment;

    CustomSlider highCutSlider;
    juce::Label highCutLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;

    juce::Slider lowCutSlider;
    juce::Label lowCutLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;

    juce::Slider rateSlider;
    juce::Label rateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;

    juce::Slider depthSlider;
    juce::Label depthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Add the getBackgroundColor method declaration
    juce::Colour getBackgroundColor();

    // Add these variables for color transition
    juce::Colour currentBackgroundColor;
    juce::Colour targetBackgroundColor;
    int colorTransitionSteps;
    int currentTransitionStep;

    std::unique_ptr<juce::MenuBarComponent> menuBar;
    juce::PopupMenu customPresetsMenu;
    void loadCustomPresetsFromDirectory();

    enum MenuIDs
    {
        loadPresetID = 1,
        savePresetID,
        putIntoPresetMenuID, // Add this line
        preset1ID = 100,
        preset2ID,
        preset3ID,
        customPresetBaseID = 1000, // Base ID for custom presets
        sizeID = 2000
    };

    enum PresetIDs
    {
        preset1 = 101,
        preset2,
        preset3
    };

    juce::StringArray customPresets;

    void putIntoPresetMenuAs();
    void addCustomPresetToMenu(const juce::String &presetName, const juce::String &presetPath);
    void loadCustomPreset(int presetID);
    void resizeEditor(float scaleFactor);

    void loadPreset();
    void savePreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(_4kverbAudioProcessorEditor)
};
