// In PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomLookAndFeel.h"

_4kverbAudioProcessorEditor::_4kverbAudioProcessorEditor(_4kverbAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&customLookAndFeel);
    setKnobColors(juce::Colours::black, juce::Colours::white, juce::Colours::black);

    menuBar = std::make_unique<juce::MenuBarComponent>(this);
    menuBar->setLookAndFeel(&customLookAndFeel);
    menuBar->setModel(this);
    addAndMakeVisible(menuBar.get());

    loadCustomPresetsFromDirectory();

    addAndMakeVisible(predelaySlider);
    predelaySlider.setSliderStyle(juce::Slider::Rotary);
    predelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);         
    predelaySlider.setRange(0.0, 500.0, 1.0);                                          
    predelaySlider.setValue(20.0f);                                                    
    predelaySlider.setLookAndFeel(&customLookAndFeel);
    predelaySlider.addListener(this);                                                  
    predelaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    predelaySlider.setSliderSnapsToMousePosition(false);                               
    predelaySlider.setTextValueSuffix(" ms");                                          
    predelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "predelay", predelaySlider);
    predelayLabel.setText("PreDelay", juce::dontSendNotification);
    predelayLabel.setJustificationType(juce::Justification::centred);
    predelayLabel.setName("PreDelayLabel");                                            
    predelayLabel.setColour(juce::Label::textColourId, juce::Colours::black);          
    predelayLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold));         
    addAndMakeVisible(predelayLabel);

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);              
    mixSlider.setRange(0.0, 1.0, 0.01);                                                
    mixSlider.setValue(1.0f);                                                          
    mixSlider.setLookAndFeel(&customLookAndFeel);     
    mixSlider.addListener(this);                                                       
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);      
    mixSlider.setSliderSnapsToMousePosition(false);                                    
    mixSlider.setTextValueSuffix(" %");                                                
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "mix", mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setName("MixLabel");                                         
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    mixLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(mixLabel);

    addAndMakeVisible(decaySlider);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    decaySlider.setRange(0.2, 70.0, 0.1);                                   
    decaySlider.setValue(4.0f);                                             
    decaySlider.setLookAndFeel(&customLookAndFeel);
    decaySlider.addListener(this);                                                  
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    decaySlider.setSliderSnapsToMousePosition(false);                               
    decaySlider.setTextValueSuffix(" s");                                           
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "decay", decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setName("DecayLabel");                                       
    decayLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    decayLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(decayLabel);

    addAndMakeVisible(sizeSlider);
    sizeSlider.setSliderStyle(juce::Slider::Rotary);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    sizeSlider.setRange(0.0, 1.0, 0.01);                                   
    sizeSlider.setValue(0.5f);                                             
    sizeSlider.setLookAndFeel(&customLookAndFeel);
    sizeSlider.addListener(this);                                                  
    sizeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    sizeSlider.setSliderSnapsToMousePosition(false);                               
    sizeSlider.setTextValueSuffix(" %");                                           
    sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "size", sizeSlider);
    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setJustificationType(juce::Justification::centred);
    sizeLabel.setName("SizeLabel");                                        
    sizeLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    sizeLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(sizeLabel);

    addAndMakeVisible(highCutSlider);
    highCutSlider.setSliderStyle(juce::Slider::Rotary);
    highCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    highCutSlider.setRange(1000.0, 21000.0, 100.0);                           
    highCutSlider.setValue(8000.0f);                                          
    highCutSlider.setLookAndFeel(&customLookAndFeel);
    highCutSlider.addListener(this);                                                  
    highCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    highCutSlider.setSliderSnapsToMousePosition(false);                               
    highCutSlider.setName("HighCut");                                                 
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "highCut", highCutSlider);
    highCutLabel.setText("High Cut", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setName("HighCutLabel");                                     
    highCutLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    highCutLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(highCutLabel);

    addAndMakeVisible(lowCutSlider);
    lowCutSlider.setSliderStyle(juce::Slider::Rotary);
    lowCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    lowCutSlider.setRange(10.0, 500.0, 10.0);                                
    lowCutSlider.setValue(10.0f);                                            
    lowCutSlider.setLookAndFeel(&customLookAndFeel);
    lowCutSlider.addListener(this);                                                  
    lowCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    lowCutSlider.setSliderSnapsToMousePosition(false);                               
    lowCutSlider.setTextValueSuffix(" Hz");                                          
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "lowCut", lowCutSlider);
    lowCutLabel.setText("Low Cut", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setName("LowCutLabel");                                      
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    lowCutLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(lowCutLabel);

    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(juce::Slider::Rotary);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    rateSlider.setRange(0.1, 10.0, 0.1);                                   
    rateSlider.setValue(1.0f);                                             
    rateSlider.setLookAndFeel(&customLookAndFeel);
    rateSlider.addListener(this);                                                  
    rateSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    rateSlider.setSliderSnapsToMousePosition(false);                               
    rateSlider.setTextValueSuffix(" Hz");                                          
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "rate", rateSlider);
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setName("RateLabel");                                        
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    rateLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(rateLabel);

    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25); 
    depthSlider.setRange(0.0, 100.0, 1.0);                                  
    depthSlider.setValue(50.0f);                                            
    depthSlider.setLookAndFeel(&customLookAndFeel);
    depthSlider.addListener(this);                                                  
    depthSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black); 
    depthSlider.setSliderSnapsToMousePosition(false);                               
    depthSlider.setTextValueSuffix(" %");                                           
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(), "depth", depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.setJustificationType(juce::Justification::centred);
    depthLabel.setName("DepthLabel");                                       
    depthLabel.setColour(juce::Label::textColourId, juce::Colours::black);  
    depthLabel.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
    addAndMakeVisible(depthLabel);
    
    currentBackgroundColor = juce::Colours::white; 
    targetBackgroundColor = currentBackgroundColor;
    colorTransitionSteps = 0;
    currentTransitionStep = 0;

    startTimer(30);

    setSize(125, 1000);
}

_4kverbAudioProcessorEditor::~_4kverbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

void _4kverbAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getBackgroundColor());

    juce::ColourGradient gradient(juce::Colours::transparentWhite, 0, 0,
                                  juce::Colours::transparentWhite, 0, getHeight(), false);

    g.setGradientFill(gradient);
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colours::black);
    g.setFont(juce::Font("Arial Black", 15.0f, juce::Font::bold)); 
}

void _4kverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 20;
    auto sliderDiameter = 100;
    auto labelWidth = 100; 

    menuBar->setBounds(bounds.removeFromTop(20));

    auto totalHeight = (sliderDiameter + labelHeight) * 8;

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

void _4kverbAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    float predelayValue = predelaySlider.getValue();
    float mixValue = mixSlider.getValue();
    float decayValue = decaySlider.getValue();
    float sizeValue = sizeSlider.getValue();
    float highCutValue = highCutSlider.getValue();
    float lowCutValue = lowCutSlider.getValue();
    float rateValue = rateSlider.getValue();
    float depthValue = depthSlider.getValue();

    float red = (predelayValue / 500.0f + mixValue + decayValue / 70.0f) / 3.0f;
    float green = (sizeValue + highCutValue / 21000.0f + lowCutValue / 500.0f) / 3.0f;
    float blue = (rateValue / 10.0f + depthValue / 100.0f) / 2.0f;

    targetBackgroundColor = juce::Colour::fromFloatRGBA(red, green, blue, 1.0f);

    colorTransitionSteps = 20; 
    currentTransitionStep = 0;

    repaint(); 
}

juce::Colour _4kverbAudioProcessorEditor::getBackgroundColor()
{
    float predelayValue = predelaySlider.getValue();
    float mixValue = mixSlider.getValue();
    float decayValue = decaySlider.getValue();
    float sizeValue = sizeSlider.getValue();
    float highCutValue = highCutSlider.getValue();
    float lowCutValue = lowCutSlider.getValue();
    float rateValue = rateSlider.getValue();
    float depthValue = depthSlider.getValue();

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

        juce::Colour newColor = currentBackgroundColor.interpolatedWith(targetBackgroundColor, alpha);

        currentBackgroundColor = newColor;
        ++currentTransitionStep;

        repaint();

        if (currentTransitionStep >= colorTransitionSteps)
        {
            currentBackgroundColor = targetBackgroundColor;
            colorTransitionSteps = 0;
            currentTransitionStep = 0;
        }
    }

    float audioLevel = audioProcessor.getAudioLevel();
    float brightness = juce::jmap(audioLevel, 0.0f, 1.0f, 0.0f, 1.0f);

    currentBackgroundColor = juce::Colour::fromHSV(currentBackgroundColor.getHue(), currentBackgroundColor.getSaturation(), brightness, 1.0f);

    repaint();
}

void _4kverbAudioProcessorEditor::loadPreset()
{
    DBG("loadPreset called");
    fileChooser = std::make_unique<juce::FileChooser>("Select a preset to load", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.preset");

    fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                             [this](const juce::FileChooser &fc)
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

                                 fileChooser.reset();
                             });
}

void _4kverbAudioProcessorEditor::savePreset()
{
    DBG("savePreset called");
    auto defaultPresetDirectory = audioProcessor.getDefaultPresetDirectory();
    fileChooser = std::make_unique<juce::FileChooser>("Save preset as...", defaultPresetDirectory, "*.preset");

    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
                             [this](const juce::FileChooser &fc)
                             {
                                 auto file = fc.getResult();
                                 if (file != juce::File{})
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

                                 fileChooser.reset();
                             });
}

juce::PopupMenu _4kverbAudioProcessorEditor::getMenuForIndex(int menuIndex, const juce::String &menuName)
{
    juce::PopupMenu menu;

    if (menuName == "File")
    {
        menu.addItem(_4kverbAudioProcessorEditor::MenuIDs::loadPresetID, "Load Preset");
        menu.addItem(_4kverbAudioProcessorEditor::MenuIDs::savePresetID, "Save Preset");
        menu.addItem(_4kverbAudioProcessorEditor::MenuIDs::putIntoPresetMenuID, "Put into Preset Menu as..."); 
    }
    else if (menuName == "Presets")
    {
        menu.addItem(PresetIDs::preset1, "Preset 1");
        menu.addItem(PresetIDs::preset2, "Preset 2");
        menu.addItem(PresetIDs::preset3, "Preset 3");
        menu.addSeparator();
        menu.addSubMenu("Custom Presets", customPresetsMenu); 
    }
    else if (menuName == "Size")
    {
        menu.addItem(2000, "75%");
        menu.addItem(2001, "100%");
        menu.addItem(2002, "125%");
        menu.addItem(2003, "150%");
        menu.addItem(2004, "200%");
    }

    return menu;
}

juce::StringArray _4kverbAudioProcessorEditor::getMenuBarNames()
{
    return {"File", "Presets", "Size"};
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
    case MenuIDs::putIntoPresetMenuID:
        DBG("Put into Preset Menu as... menu item selected");
        putIntoPresetMenuAs();
        break;
    case PresetIDs::preset1:
        predelaySlider.setValue(50.0f);
        decaySlider.setValue(5.0f);
        sizeSlider.setValue(0.7f);
        highCutSlider.setValue(10000.0f);
        lowCutSlider.setValue(50.0f);
        rateSlider.setValue(2.0f);
        depthSlider.setValue(30.0f);
        break;
    case PresetIDs::preset2:
        predelaySlider.setValue(100.0f);
        decaySlider.setValue(10.0f);
        sizeSlider.setValue(0.5f);
        highCutSlider.setValue(15000.0f);
        lowCutSlider.setValue(100.0f);
        rateSlider.setValue(3.0f);
        depthSlider.setValue(40.0f);
        break;
    case PresetIDs::preset3:
        predelaySlider.setValue(200.0f);
        decaySlider.setValue(20.0f);
        sizeSlider.setValue(0.9f);
        highCutSlider.setValue(8000.0f);
        lowCutSlider.setValue(20.0f);
        rateSlider.setValue(1.0f);
        depthSlider.setValue(50.0f);
        break;
    case MenuIDs::sizeID:
        switch (menuItemID)
        {
        case 2000:
            resizeEditor(0.75f);
            break;
        case 2001:
            resizeEditor(1.0f);
            break;
        case 2002:
            resizeEditor(1.25f);
            break;
        case 2003:
            resizeEditor(1.5f);
            break;
        case 2004:
            resizeEditor(2.0f);
            break;
        }
        break;
    default:
        if (menuItemID >= MenuIDs::customPresetBaseID)
        {
            loadCustomPreset(menuItemID);
        }
        break;
    }
}

void _4kverbAudioProcessorEditor::putIntoPresetMenuAs()
{
    auto defaultPresetDirectory = audioProcessor.getDefaultPresetDirectory();
    fileChooser = std::make_unique<juce::FileChooser>("Save preset as...", defaultPresetDirectory, "*.preset");

    fileChooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
                             [this](const juce::FileChooser &fc)
                             {
                                 auto file = fc.getResult();
                                 if (file != juce::File{}) 
                                 {
                                     DBG("File selected: " + file.getFullPathName());
                                     juce::MemoryBlock data;
                                     audioProcessor.getStateInformation(data);
                                     auto result = file.replaceWithData(data.getData(), data.getSize());
                                     if (result)
                                     {
                                         DBG("Preset saved to file: " + file.getFullPathName());
                                         addCustomPresetToMenu(file.getFileNameWithoutExtension(), file.getFullPathName());
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
                                 fileChooser.reset();
                             });
}

void _4kverbAudioProcessorEditor::addCustomPresetToMenu(const juce::String &presetName, const juce::String &presetPath)
{
    if (!customPresets.contains(presetPath))
    {
        customPresets.add(presetPath);
        int presetID = MenuIDs::customPresetBaseID + customPresets.size() - 1;
        customPresetsMenu.addItem(presetID, presetName);
    }
}

void _4kverbAudioProcessorEditor::loadCustomPreset(int presetID)
{
    int index = presetID - MenuIDs::customPresetBaseID;
    if (index >= 0 && index < customPresets.size())
    {
        juce::File file(customPresets[index]);
        if (file.existsAsFile())
        {
            juce::FileInputStream inputStream(file);
            if (inputStream.openedOk())
            {
                juce::MemoryBlock data;
                inputStream.readIntoMemoryBlock(data);
                audioProcessor.setStateInformation(data.getData(), static_cast<int>(data.getSize()));
                DBG("Custom preset loaded from file: " + file.getFullPathName());
            }
        }
    }
}

void _4kverbAudioProcessorEditor::loadCustomPresetsFromDirectory()
{
    auto presetDirectory = audioProcessor.getDefaultPresetDirectory();
    if (presetDirectory.exists() && presetDirectory.isDirectory())
    {
        auto presetFiles = presetDirectory.findChildFiles(juce::File::findFiles, false, "*.preset");
        for (auto &file : presetFiles)
        {
            addCustomPresetToMenu(file.getFileNameWithoutExtension(), file.getFullPathName());
        }
    }
}

void _4kverbAudioProcessorEditor::resizeEditor(float scaleFactor)
{
    auto currentWidth = getWidth();
    auto currentHeight = getHeight();

    auto newWidth = static_cast<int>(currentWidth * scaleFactor);
    auto newHeight = static_cast<int>(currentHeight * scaleFactor);

    setSize(newWidth, newHeight);
}
