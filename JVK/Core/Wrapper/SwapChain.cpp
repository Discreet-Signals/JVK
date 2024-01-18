/*
  ==============================================================================

    SwapChain.cpp
    Created: 13 Oct 2023 2:38:19pm
    Author:  Gavin

  ==============================================================================
*/

#include "SwapChain.h"

namespace jvk::core
{

SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, info.surface, &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, info.surface, &formatCount, nullptr);
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, info.surface, &formatCount, details.formats.data());
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, info.surface, &presentModeCount, nullptr);
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, info.surface, &presentModeCount, details.presentModes.data());
    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availableMode : availablePresentModes) {
        if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) return availableMode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;
    else
        return { static_cast<uint32_t>(std::fmax(capabilities.minImageExtent.width, std::fmin(capabilities.maxImageExtent.width, info.size.x))),
                 static_cast<uint32_t>(std::fmax(capabilities.minImageExtent.height, std::fmin(capabilities.maxImageExtent.height, info.size.y))) };
}

SwapChain::SwapChain(SwapChainInfo sc_info, VkSwapchainKHR previous) : info(sc_info)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(info.physicalDevice );
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D swapExtent = chooseSwapExtent(swapChainSupport.capabilities);
    
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;
    
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = info.surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = swapExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    uint32_t queueFamilyIndices[] = { info.graphicsQueueFamilyIndex, info.presentQueueFamilyIndex };
    if (info.graphicsQueueFamilyIndex != info.presentQueueFamilyIndex)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    
    // If we're recreating the swap chain, reference the old one (useful for window resizing, etc.)
    createInfo.oldSwapchain = previous; // No old swap chain in this instance.
    
    vkCreateSwapchainKHR(info.device, &createInfo, nullptr, &swapChain);
    
    // Retrieve swap chain images
    vkGetSwapchainImagesKHR(info.device, swapChain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(info.device, swapChain, &imageCount, images.data());
    
    // Store the chosen format and extent for future use (rendering, creating image views, etc.)
    format = surfaceFormat.format;
    extent = swapExtent;
    
    createMsaaImages();
    createImageViews();
    
    if (info.renderPass)
        createFrameBuffers(info.renderPass);
}

void SwapChain::createMsaaImages()
{
    msaaImages.resize(images.size());
    msaaImageMemory.resize(images.size());

    for (size_t i = 0; i < images.size(); i++)
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent.width = extent.width;
        imageInfo.extent.height = extent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_4_BIT; // Change this to the desired msaa count
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        vkCreateImage(info.device, &imageInfo, nullptr, &msaaImages[i]);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(info.device, msaaImages[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(info.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vkAllocateMemory(info.device, &allocInfo, nullptr, &msaaImageMemory[i]);

        vkBindImageMemory(info.device, msaaImages[i], msaaImageMemory[i], 0);
    }

}


void SwapChain::createImageViews()
{
    msaaImageViews.resize(msaaImages.size());
    for (size_t i = 0; i < msaaImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = msaaImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format; // Assuming the msaad images have the same format as your swap chain images
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        vkCreateImageView(info.device, &createInfo, nullptr, &msaaImageViews[i]);
    }
    imageViews.resize(images.size());
    for (size_t i = 0; i < images.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format; // This is the format you picked earlier in chooseSwapSurfaceFormat
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        vkCreateImageView(info.device, &createInfo, nullptr, &imageViews[i]);
    }
}

void SwapChain::createFrameBuffers(VkRenderPass renderPass)
{
    // Resize the framebuffers vector to match the number of swap chain image views
    frameBuffers.resize(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); i++)
    {
        VkImageView attachments[] = {
            msaaImageViews[i],
            imageViews[i]
            // TODO: depthBuffer
        };
        
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;  // Assumes you've created a renderPass member variable previously
        framebufferInfo.attachmentCount = static_cast<uint32_t>(std::size(attachments));
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent.width;  // Assumes swapChainExtent contains the size of your swap chain images
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;
        
        vkCreateFramebuffer(info.device, &framebufferInfo, nullptr, &frameBuffers[i]);
    }
}

SwapChain::~SwapChain()
{
    for (auto framebuffer : frameBuffers)
        vkDestroyFramebuffer(info.device, framebuffer, nullptr);
    for (auto imageView : imageViews)
        vkDestroyImageView(info.device, imageView, nullptr);
    for (auto imageView : msaaImageViews)
        vkDestroyImageView(info.device, imageView, nullptr);
    for (auto image : msaaImages)
        vkDestroyImage(info.device, image, nullptr);
    for (auto memory : msaaImageMemory)
        vkFreeMemory(info.device, memory, nullptr);
    vkDestroySwapchainKHR(info.device, swapChain, nullptr);
}

} // jvk::core
