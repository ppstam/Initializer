/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InitializerAudioProcessor::InitializerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    treeState(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

InitializerAudioProcessor::~InitializerAudioProcessor()
{
}

//==============================================================================
const juce::String InitializerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool InitializerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool InitializerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool InitializerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double InitializerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int InitializerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int InitializerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void InitializerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String InitializerAudioProcessor::getProgramName(int index)
{
    return {};
}

void InitializerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void InitializerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void InitializerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool InitializerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void InitializerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        auto sliderGainValue = treeState.getRawParameterValue(GAIN_ID);
        auto dbToGain = juce::Decibels::decibelsToGain(sliderGainValue->load());

        // mono in
        if (buffer.getNumChannels() == 1)
        {
            float sampleLeft = buffer.getSample(0, sample);
            float new_sampleLeft = sampleLeft;

            // phase reverse in mono
            if (getPhaseReverse())
            {
                new_sampleLeft = -sampleLeft;
            }

            buffer.setSample(0, sample, new_sampleLeft * dbToGain);
        }

        // stereo in
        if (buffer.getNumChannels() == 2)
        {
            float sampleLeft = buffer.getSample(0, sample);
            float sampleRight = buffer.getSample(1, sample);

            float pSelectedSampleLeft = sampleLeft;
            float pSelectedSampleRight = sampleRight;

            float newSampleLeft = sampleLeft;
            float newSampleRight = sampleRight;

            // mid solo
            if (getMidSolo()) {
                pSelectedSampleLeft = (sampleLeft + sampleRight) / 2.0;
                pSelectedSampleRight = (sampleLeft + sampleRight) / 2.0;
            }

            // side solo
            if (getSideSolo()) {
                pSelectedSampleLeft = (sampleLeft - sampleRight) / 2.0;
                pSelectedSampleRight = -(sampleLeft - sampleRight) / 2.0;
            }

            //left solo
            if (getLeftSolo()) {
                pSelectedSampleLeft = sampleLeft;
                pSelectedSampleRight = 0.0;
            }

            //right solo
            if (getRightSolo()) {
                pSelectedSampleLeft = 0.0;;
                pSelectedSampleRight = sampleRight;
            }

            //stereo
            if (getStereoSolo()) {
                pSelectedSampleLeft = sampleLeft;
                pSelectedSampleRight = sampleRight;
            }

            // phase reverse in stereo
            if (getPhaseReverse())
            {
                newSampleLeft = -pSelectedSampleLeft;
                newSampleRight = -pSelectedSampleRight;
            }
            else if (getStereoFlip())
            {
                newSampleLeft = pSelectedSampleRight;
                newSampleRight = pSelectedSampleLeft;
            }
            else {
                newSampleLeft = pSelectedSampleLeft;
                newSampleRight = pSelectedSampleRight;
            }

            buffer.setSample(0, sample, newSampleLeft * dbToGain);
            buffer.setSample(1, sample, newSampleRight * dbToGain);
        }
    }
}

//==============================================================================
bool InitializerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* InitializerAudioProcessor::createEditor()
{
    return new InitializerAudioProcessorEditor(*this);
}

//==============================================================================
void InitializerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void InitializerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InitializerAudioProcessor();
}


bool InitializerAudioProcessor::getPhaseReverse()
{
    return phaseReverse;
}

void InitializerAudioProcessor::setPhaseReverse(bool ph)
{
    phaseReverse = ph;
}

bool InitializerAudioProcessor::getStereoFlip()
{
    return stereoFlip;
}

void InitializerAudioProcessor::setStereoFlip(bool stFlip)
{
    stereoFlip = stFlip;
}

bool InitializerAudioProcessor::getMidSolo() {
    return midSolo;
}

void InitializerAudioProcessor::setMidSolo(bool mid) {
    midSolo = mid;
}

bool InitializerAudioProcessor::getSideSolo() {
    return sideSolo;
}

void InitializerAudioProcessor::setSideSolo(bool side) {
    sideSolo = side;
}

bool InitializerAudioProcessor::getLeftSolo()
{
    return leftSolo;
}

void InitializerAudioProcessor::setLeftSolo(bool left) {
    leftSolo = left;
}

bool InitializerAudioProcessor::getRightSolo() {
    return rightSolo;
}

void InitializerAudioProcessor::setRightSolo(bool right) {
    rightSolo = right;
}

bool InitializerAudioProcessor::getStereoSolo() {
    return stereoSolo;
}

void InitializerAudioProcessor::setStereoSolo(bool stereo) {
    stereoSolo = stereo;
}

juce::AudioProcessorValueTreeState::ParameterLayout InitializerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(GAIN_ID, GAIN_NAME, -60.0f, 12.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>(PHASE_REV_ID, PHASE_REV_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(STEREO_FLIP_ID, STEREO_FLIP_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(MID_SOLO_ID, MID_SOLO_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(SIDE_SOLO_ID, SIDE_SOLO_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(LEFT_SOLO_ID, LEFT_SOLO_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(RIGHT_SOLO_ID, RIGHT_SOLO_NAME, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(STEREO_SOLO_ID, STEREO_SOLO_NAME, true));

    return layout;
}


