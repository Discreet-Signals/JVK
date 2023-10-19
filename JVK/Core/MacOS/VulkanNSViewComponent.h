/*
  ==============================================================================

    VulkanNSViewComponent.h
    Created: 11 Oct 2023 1:51:28pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "../VulkanInstance.h"
#include "NSViewGenerator.h"

namespace jvk::core::macos
{

class VulkanNSViewComponent : public VulkanInstance, public juce::NSViewComponent
{
public:
    virtual void jvkResized() {}
private:
    std::vector<const char*> getExtensions() override;
    void createSurface() override;
    void resized() final override;
    NSViewGenerator nsView;
};

} // jvk::core::macos
