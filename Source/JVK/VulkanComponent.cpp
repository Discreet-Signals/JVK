/*
  ==============================================================================

    VulkanComponent.cpp
    Created: 10 Oct 2023 12:53:20pm
    Author:  Gavin

  ==============================================================================
*/

#include <fstream>
#include "VulkanComponent.h"

namespace jvk
{

VulkanComponent::VulkanComponent()
{
    initializeVulkan();
    startTimerHz(60);
}

} // jvk
