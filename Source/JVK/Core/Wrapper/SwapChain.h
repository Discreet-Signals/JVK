/*
  ==============================================================================

    SwapChain.h
    Created: 13 Oct 2023 2:29:41pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace jvk::core
{

static inline uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    // Handle Error
    return 0;
};


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChainInfo
{
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    glm::vec2 size;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
    VkRenderPass renderPass = VK_NULL_HANDLE;
};

class SwapChain
{
public:
    SwapChain(SwapChainInfo sc_info);
    ~SwapChain();
    
    VkSwapchainKHR getInternal() const { return swapChain; };
    VkFormat getFormat() const { return format; };
    VkExtent2D getExtent() const { return extent; };
    int getNumImages() const { return static_cast<int>(images.size()); }
    int getWidth() const { return info.size.x; };
    int getHeight() const { return info.size.y; };
    
    void createFrameBuffers(VkRenderPass renderPass);
    VkFramebuffer getFrameBuffer(int i) { return frameBuffers[i]; };
private:
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    
    void createMsaaImages();
    void createImageViews();
    
    SwapChainInfo info;
    VkSwapchainCreateInfoKHR createInfo;
    
    // Owned
    VkSwapchainKHR swapChain;
    std::vector<VkImage> msaaImages;
    std::vector<VkDeviceMemory> msaaImageMemory;
    std::vector<VkImageView> msaaImageViews;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    VkFormat format;
    VkExtent2D extent;
    std::vector<VkFramebuffer> frameBuffers;
};

} // jvk::core
