/*
  ==============================================================================

    VulkanHWNDComponent.cpp
    Created: 11 Oct 2023 1:52:05pm
    Author:  Gavin

  ==============================================================================
*/
#include "VulkanHWNDComponent.h"
namespace jvk::core::windows
{

std::vector<const char*> VulkanHWNDComponent::getExtensions()
{
    return {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };
}
void VulkanHWNDComponent::createSurface()
{
    HWND& hwndRef = hwnd.create();
    setHWND(hwndRef);

    DBG("Creating Surface...");
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hwnd = hwndRef;
    if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS)
    {
        DBG("Failed to create a Vulkan surface!");
        return;
    }
    DBG("Created Surface!");
}

void VulkanHWNDComponent::resized()
{
    resizeToFit();
    width.store(getWidth());
    height.store(getHeight());
    windowResized.store(true);
    jvkResized();
}

} // jvk::core::windows