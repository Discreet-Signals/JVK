/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceVulkanAudioProcessorEditor::JuceVulkanAudioProcessorEditor (JuceVulkanAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(vkComponent);
    setSize (800, 600);
    setResizable(true, true);
}

JuceVulkanAudioProcessorEditor::~JuceVulkanAudioProcessorEditor()
{
}

//==============================================================================
void JuceVulkanAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void JuceVulkanAudioProcessorEditor::resized()
{
    vkComponent.setBounds(getBounds());
}
