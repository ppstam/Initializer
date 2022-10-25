/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

enum RadioButtonsIds
{
    soloButtonsRadioGroupID = 1001
};

//==============================================================================
InitializerAudioProcessorEditor::InitializerAudioProcessorEditor(InitializerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    initGainSlider();
    initSoloButtons();

    //Slider label
    addAndMakeVisible(gainSliderLabel);
    gainSliderLabel.setText("Trim", juce::NotificationType::dontSendNotification);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    gainSliderLabel.setJustificationType(juce::Justification::centred);

    // Purple colour
    auto purpleHue = juce::Colours::purple.getHue();

    // Phase Button
    phaseButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, PHASE_REV_ID, phaseButton);
    phaseButton.setButtonText(PHASE_REV_NAME);
    addAndMakeVisible(phaseButton);
    phaseButton.addListener(this);
    phaseButton.setClickingTogglesState(true);

    phaseButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromHSV(purpleHue, 0.3f, 0.2f, 1.0f));
    phaseButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromHSV(purpleHue, 0.3f, 0.4f, 1.0f));

    //Stereo Flip Button
    stereoFlipButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, STEREO_FLIP_ID, stereoFlipButton);
    stereoFlipButton.setButtonText(STEREO_FLIP_NAME);
    addAndMakeVisible(stereoFlipButton);
    stereoFlipButton.addListener(this);
    stereoFlipButton.setClickingTogglesState(true);

    stereoFlipButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromHSV(purpleHue, 0.3f, 0.2f, 1.0f));
    stereoFlipButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromHSV(purpleHue, 0.3f, 0.4f, 1.0f));

    setSize(400, 300);
}

InitializerAudioProcessorEditor::~InitializerAudioProcessorEditor()
{
}

//==============================================================================
void InitializerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.brighter(0.1));

    g.setColour(juce::Colours::whitesmoke);
    g.setFont(36.0f);
    g.drawFittedText("Initializer", getLocalBounds(), juce::Justification::centredTop, 1);
}

void InitializerAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.3;
    auto sliderSize = getWidth() * 0.4;
    auto buttonWidth = getWidth() * 0.4;
    auto buttonHeight = getWidth() * 0.07;
    auto heightFactor = buttonHeight * 1.2;

    gainSlider.setBounds(leftMargin, topMargin, sliderSize, sliderSize);
    phaseButton.setBounds(gainSlider.getX() + gainSlider.getWidth() + leftMargin, topMargin, buttonWidth, buttonHeight);
    stereoFlipButton.setBounds(gainSlider.getX() + gainSlider.getWidth() + leftMargin, topMargin + heightFactor, buttonWidth, buttonHeight);

    midSoloButton.setBounds(phaseButton.getX(), topMargin + 2.0 * heightFactor, buttonWidth, buttonHeight);
    sideSoloButton.setBounds(phaseButton.getX(), topMargin + 3.0 * heightFactor, buttonWidth, buttonHeight);

    leftSoloButton.setBounds(phaseButton.getX() + 10.0 * leftMargin, topMargin + 2.0 * heightFactor, buttonWidth, buttonHeight);
    rightSoloButton.setBounds(phaseButton.getX() + 10.0 * leftMargin, topMargin + 3.0 * heightFactor, buttonWidth, buttonHeight);

    stereoButton.setBounds(phaseButton.getX() + 5.0 * leftMargin, topMargin + 4.0 * heightFactor, buttonWidth, buttonHeight);
}

void InitializerAudioProcessorEditor::initSoloButtons()
{
    addAndMakeVisible(midSoloButton);
    addAndMakeVisible(sideSoloButton);
    addAndMakeVisible(leftSoloButton);
    addAndMakeVisible(rightSoloButton);
    addAndMakeVisible(stereoButton);

    midSoloButton.setButtonText(MID_SOLO_NAME);
    sideSoloButton.setButtonText(SIDE_SOLO_NAME);
    leftSoloButton.setButtonText(LEFT_SOLO_NAME);
    rightSoloButton.setButtonText(RIGHT_SOLO_NAME);
    stereoButton.setButtonText(STEREO_SOLO_NAME);

    midSoloButton.addListener(this);
    sideSoloButton.addListener(this);
    leftSoloButton.addListener(this);
    rightSoloButton.addListener(this);
    stereoButton.addListener(this);

    midSoloButton.setRadioGroupId(soloButtonsRadioGroupID);
    sideSoloButton.setRadioGroupId(soloButtonsRadioGroupID);
    leftSoloButton.setRadioGroupId(soloButtonsRadioGroupID);
    rightSoloButton.setRadioGroupId(soloButtonsRadioGroupID);
    stereoButton.setRadioGroupId(soloButtonsRadioGroupID);

    midSoloButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, MID_SOLO_ID, midSoloButton);
    sideSoloButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, SIDE_SOLO_ID, sideSoloButton);
    leftSoloButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, LEFT_SOLO_ID, leftSoloButton);
    rightSoloButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, RIGHT_SOLO_ID, rightSoloButton);
    stereoButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, STEREO_SOLO_ID, stereoButton);
}


void InitializerAudioProcessorEditor::initGainSlider()
{
    // Slider 
    addAndMakeVisible(gainSlider);
    sliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
    gainSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withHue(0.5));
    gainSlider.setDoubleClickReturnValue(true, 0.0);
}


void InitializerAudioProcessorEditor::setRestSoloButtonsOff(juce::Button* buttonClicked)
{
    for (juce::Button* button : mutuallyExclusiveButtons) {

    }

}


void InitializerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &phaseButton)
    {
        audioProcessor.setPhaseReverse(!audioProcessor.getPhaseReverse());
    }
    if (button == &stereoFlipButton)
    {
        audioProcessor.setStereoFlip(!audioProcessor.getStereoFlip());
    }
    if (button == &midSoloButton) {
        audioProcessor.setMidSolo(true);
        audioProcessor.setSideSolo(false);
        audioProcessor.setLeftSolo(false);
        audioProcessor.setRightSolo(false);
        audioProcessor.setStereoSolo(false);
    }
    if (button == &sideSoloButton) {
        audioProcessor.setMidSolo(false);
        audioProcessor.setSideSolo(true);
        audioProcessor.setLeftSolo(false);
        audioProcessor.setRightSolo(false);
        audioProcessor.setStereoSolo(false);
    }
    if (button == &leftSoloButton) {
        audioProcessor.setMidSolo(false);
        audioProcessor.setSideSolo(false);
        audioProcessor.setLeftSolo(true);
        audioProcessor.setRightSolo(false);
        audioProcessor.setStereoSolo(false);
    }
    if (button == &rightSoloButton) {
        audioProcessor.setMidSolo(false);
        audioProcessor.setSideSolo(false);
        audioProcessor.setLeftSolo(false);
        audioProcessor.setRightSolo(true);
        audioProcessor.setStereoSolo(false);
    }
    if (button == &stereoButton) {
        audioProcessor.setMidSolo(false);
        audioProcessor.setSideSolo(false);
        audioProcessor.setLeftSolo(false);
        audioProcessor.setRightSolo(false);
        audioProcessor.setStereoSolo(true);
    }
}


