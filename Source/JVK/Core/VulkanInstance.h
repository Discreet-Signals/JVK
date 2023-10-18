/*
  ==============================================================================

    VulkanInstance.h
    Created: 11 Oct 2023 1:53:22pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Wrapper/Wrapper.h"

namespace jvk::core
{

class VulkanInstance
{
public:
    VulkanInstance() { };
    virtual ~VulkanInstance() { };
    
protected:
    virtual void renderComponents(VkCommandBuffer& commandBuffer) { };
    
    virtual std::vector<const char*> getExtensions() { return {}; };
    virtual void createSurface() { };
    
    void initializeVulkan();
    void createInstance();
    void setupDebugMessenger();
    bool isDeviceSuitable(VkPhysicalDevice device);
    void selectPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();
    void createSwapChain();
    void createRenderPass();
    void createGraphicsPipeline();
    void createCommandBuffers();
    void createSyncObjects();
    void submitCommandBuffer(int index);
    void checkForResize();
    void execute();
    void release();
    
    // Vulkan core components
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger; // if using validation layers
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    
    // Swapchain components
    std::unique_ptr<SwapChain> swapChain;
    
    // Graphics pipeline components
    VkRenderPass renderPass;
    std::unique_ptr<Pipeline> defaultPipeline;
    VkViewport viewport = {};
    VkRect2D scissor = {};
    
    // Drawing components
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    
    // Synchronization components
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<VkFence> imagesInFlight;
    
    // Queues
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    uint32_t graphicsQueueFamilyIndex = UINT32_MAX; // Initialized to an invalid index
    uint32_t presentQueueFamilyIndex = UINT32_MAX;  // Initialized to an invalid index
    
    // Other required members
    juce::Colour clearColor { juce::Colours::transparentBlack };
    size_t totalFrames = 0;
    size_t currentFrame = 0; // For keeping track of the current frame in flight
    std::atomic<int> width { 800 };
    std::atomic<int> height { 600 };
    std::atomic<bool> windowResized { false }; // Flag for window resizing
    
};

} // jvk::core
