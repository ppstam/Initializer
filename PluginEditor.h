
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class InitializerAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::TextButton::Listener
{
public:
    InitializerAudioProcessorEditor(InitializerAudioProcessor&);
    ~InitializerAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;

private:
    juce::Label gainSliderLabel;
    juce::Slider gainSlider;
    juce::TextButton phaseButton;
    juce::TextButton stereoFlipButton;
    juce::ToggleButton midSoloButton;
    juce::ToggleButton sideSoloButton;
    juce::ToggleButton leftSoloButton;
    juce::ToggleButton rightSoloButton;
    juce::ToggleButton stereoButton;

    std::vector<juce::Button*> mutuallyExclusiveButtons = { &midSoloButton, &sideSoloButton, &leftSoloButton, &rightSoloButton };


    void initSoloButtons();
    void initGainSlider();
    void setRestSoloButtonsOff(juce::Button*);

    InitializerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InitializerAudioProcessorEditor)

public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> phaseButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> stereoFlipButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> midSoloButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> sideSoloButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> leftSoloButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> rightSoloButtonAttach;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> stereoButtonAttach;
};
