/*
  ==============================================================================

    VulkanComponent.h
    Created: 8 Oct 2023 4:35:57pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "MacOS/VulkanNSViewComponent.h"

namespace jvk
{

class VulkanComponent :
    public VulkanNSViewComponent,
    public juce::Timer
{
public:
    VulkanComponent();
    ~VulkanComponent() { };

protected:
    void render(VkCommandBuffer& commandBuffer) override { };
private:
    void timerCallback() override { checkForResize(); execute(); }

};

} // jvk
