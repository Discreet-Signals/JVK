/*
  ==============================================================================

    VulkanHWNDComponent.h
    Created: 11 Oct 2023 1:51:28pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "../VulkanInstance.h"
#include <windows.h>

namespace jvk::core::windows
{

class VulkanHWNDComponent : public VulkanInstance, public juce::HWNDComponent
{
public:
    virtual void jvkResized() {}
private:
    std::vector<const char*> getExtensions() override;
    void createSurface() override;
    void resized() final override;
    HWND hwnd;
};

} // jvk::core::windows