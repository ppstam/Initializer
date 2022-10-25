/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define PHASE_REV_ID "phase_reverse"
#define PHASE_REV_NAME "Phase Reverse"
#define STEREO_FLIP_ID "stereo_flip"
#define STEREO_FLIP_NAME "Stereo Flip"
#define MID_SOLO_ID "mid"
#define MID_SOLO_NAME "Mids"
#define SIDE_SOLO_ID "side"
#define SIDE_SOLO_NAME "Sides"
#define LEFT_SOLO_ID "left"
#define LEFT_SOLO_NAME "Left"
#define RIGHT_SOLO_ID "right"
#define RIGHT_SOLO_NAME "Right"
#define STEREO_SOLO_ID "stereo"
#define STEREO_SOLO_NAME "Stereo"

//==============================================================================
/**
*/
class InitializerAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    InitializerAudioProcessor();
    ~InitializerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    bool getPhaseReverse();
    void setPhaseReverse(bool);
    bool getStereoFlip();
    void setStereoFlip(bool);
    void setMidSolo(bool);
    bool getMidSolo();
    void setSideSolo(bool);
    bool getSideSolo();
    void setLeftSolo(bool);
    bool getLeftSolo();
    void setRightSolo(bool);
    bool getRightSolo();
    void setStereoSolo(bool);
    bool getStereoSolo();


    juce::AudioProcessorValueTreeState treeState;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
    //==============================================================================
    bool phaseReverse = false;
    bool stereoFlip = false;
    bool midSolo = false;
    bool sideSolo = false;
    bool leftSolo = false;
    bool rightSolo = false;
    bool stereoSolo = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InitializerAudioProcessor)
};
