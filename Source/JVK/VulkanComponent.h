/*
  ==============================================================================

    VulkanComponent.h
    Created: 8 Oct 2023 4:35:57pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#if JUCE_MAC
#include "Core/MacOS/VulkanNSViewComponent.h"
#elif JUCE_WINDOWS
#include "Core/Windows/VulkanHWNDComponent.h"
#else
#error "JVK: Unsupported Platform!"
#endif

namespace jvk
{

class VulkanSubComponent
{
public:
    VulkanSubComponent() { }
    ~VulkanSubComponent() { }
    
    void setBounds(juce::Rectangle<int> new_bounds) { bounds = new_bounds; }
    void setBounds(int x, int y, int width, int height) { setBounds({x,y,width,height}); }
private:
    juce::Rectangle<int> bounds;
    VkPipeline pipeline;
};

class VulkanComponent :
    public core::macos::VulkanNSViewComponent,
    public juce::Timer
{
public:
    VulkanComponent();
    ~VulkanComponent() { };
    
protected:
    void render(VkCommandBuffer& commandBuffer) override { };
private:
    void timerCallback() override { checkForResize(); execute(); }
    std::vector<VulkanSubComponent*> components;
};

} // jvk
