/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TriangleDemo.h"
#include "SceneDemo.h"
//==============================================================================
/**
*/
class JuceVulkanAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuceVulkanAudioProcessorEditor (JuceVulkanAudioProcessor&);
    ~JuceVulkanAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceVulkanAudioProcessor& audioProcessor;
    jvk::VulkanRenderer jvkRenderer;
    TriangleDemo tlTriangle;
    TriangleDemo brTriangle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceVulkanAudioProcessorEditor)
};
