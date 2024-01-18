/*
  ==============================================================================

    VulkanInstance.cpp
    Created: 11 Oct 2023 1:53:22pm
    Author:  Gavin

  ==============================================================================
*/

#include "VulkanInstance.h"

namespace jvk::core
{

void VulkanInstance::initializeVulkan()
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    selectPhysicalDevice();
    createLogicalDevice();
    createCommandPool();
    createSwapChain();
    createRenderPass();
    createGraphicsPipeline();
    createCommandBuffers();
    createSyncObjects();
}

void VulkanInstance::createInstance()
{
    // 1. Vulkan instance creation with required extensions
    DBG("Creating Vulkan Instance...");
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "JUCE Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    std::vector<const char*> extensions = getExtensions();
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result == VK_SUCCESS)
        DBG("Created Vulkan Instance!");
    else
        DBG("Failed to Create Vulkan Instance: " << result);
    
}

void VulkanInstance::setupDebugMessenger()
{
    // 2. Setup debug messenger (if using validation layers)
}

bool VulkanInstance::isDeviceSuitable(VkPhysicalDevice device)
{
    // Here, we'll simply check if the device can handle graphics and presentation. Extend as needed.
    
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    bool graphicsFound = false, presentFound = false;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsQueueFamilyIndex = i;
            graphicsFound = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport)
        {
            presentQueueFamilyIndex = i;
            presentFound = true;
        }
        if (graphicsFound && presentFound)
            break;
        i++;
    }
    
    return graphicsFound && presentFound;
}

void VulkanInstance::selectPhysicalDevice()
{
    // 4. Physical device selection (based on some criteria like discrete GPU, support for required features, etc.)
    DBG("Selecting GPU...");
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0)
        DBG("Failed to find GPUs with Vulkan support!");
    
    std::vector<VkPhysicalDevice> availableDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, availableDevices.data());
    
    for (const auto& device : availableDevices)
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    
    if (physicalDevice == VK_NULL_HANDLE)
        DBG("Failed to find Suitable GPU!");
    else
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        DBG("Selected GPU: " << deviceProperties.deviceName);
    }
}

void VulkanInstance::createLogicalDevice()
{
    // 5. Logical device (VkDevice) creation with queue families for graphics, presentation, etc.
    DBG("Creating Logical Device...");
    // Define the priorities for our queues (for this example, just a single queue)
    float queuePriority = 1.0f;
    
    // Queue creation information
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex; // Assuming you have this from earlier
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    // Logical device creation information
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    
    // Enable any required device extensions here (e.g., swap chain)
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    // Optional: device-specific features you want to use
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures; // If not specific features are required, this can be nullptr
    
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) == VK_SUCCESS)
        DBG("Created Logical Device!");
    else
        DBG("Failed to create logical device!");
    
    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, presentQueueFamilyIndex, 0, &presentQueue);
}

void VulkanInstance::createSwapChain()
{
    vkDeviceWaitIdle(device);
    
    for (size_t i = 0; i < commandBuffers.size(); i++)
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffers[i]);
    
    SwapChainInfo info;
    info.physicalDevice = physicalDevice;
    info.device = device;
    info.surface = surface;
    info.size = { width.load(), height.load() };
    info.graphicsQueueFamilyIndex = graphicsQueueFamilyIndex;
    info.presentQueueFamilyIndex = presentQueueFamilyIndex;
    info.renderPass = renderPass;
    VkSwapchainKHR previous = swapChain ? swapChain->getInternal() : VK_NULL_HANDLE;
    std::unique_ptr<SwapChain> next = std::make_unique<SwapChain>(info, previous);
    swapChain = std::move(next);
    //swapChain = std::make_unique<SwapChain>(info, previous);
    
    if (renderPass)
        createCommandBuffers();
    
    viewport.width = (float) swapChain->getWidth();
    viewport.height = (float) swapChain->getHeight();
    scissor.extent = swapChain->getExtent();
}

void VulkanInstance::createRenderPass()
{
    DBG("Creating Render Pass...");
    
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChain->getFormat(); // The format you picked earlier
    colorAttachment.samples = VK_SAMPLE_COUNT_4_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear the color buffer before drawing
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Store the result after drawing
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // The layout to be ready for presentation after render pass
    VkAttachmentReference colorAttachmentRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    VkAttachmentDescription resolveAttachment = {};
    resolveAttachment.format = swapChain->getFormat();
    resolveAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // Single-sampled for the resolve attachment
    resolveAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    resolveAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    resolveAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    resolveAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    resolveAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    resolveAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    VkAttachmentReference resolveAttachmentRef = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // Index 1 = resolve attachment
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pResolveAttachments = &resolveAttachmentRef;
    
    VkAttachmentDescription attachments[2] = { colorAttachment, resolveAttachment };
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = attachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) == VK_SUCCESS)
        DBG("Created Render Pass!");
    else
        DBG("Failed to Create Render Pass!");
    
    swapChain->createFrameBuffers(renderPass);
}

void VulkanInstance::createGraphicsPipeline()
{
    Shaders::ShaderGroup defaultShaderGroup;
    defaultShaderGroup.addShader(VK_SHADER_STAGE_VERTEX_BIT,
                                 Shaders::DefaultShaders::BasicShader::basic_vert_spv,
                                 Shaders::DefaultShaders::BasicShader::basic_vert_spvSize);
    defaultShaderGroup.addShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                                 Shaders::DefaultShaders::BasicShader::basic_frag_spv,
                                 Shaders::DefaultShaders::BasicShader::basic_frag_spvSize);
    defaultPipeline = std::make_unique<Pipeline>(device, renderPass, std::move(defaultShaderGroup));
}

void VulkanInstance::createCommandPool()
{
    DBG("Creating Command Pool...");
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;  // The queue family for which this command pool is being created.
    poolInfo.flags = 0;  // You might also use flags like VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT if needed.
    
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) == VK_SUCCESS)
        DBG("Created Command Pool!");
    else
        DBG("Failed to Create Command Pool!");
}

void VulkanInstance::createCommandBuffers()
{
    commandBuffers.resize(swapChain->getNumImages());
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();
    
    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        DBG("Failed to allocate command buffers!");
}


void VulkanInstance::createSyncObjects()
{
    // 13. Create semaphores and fences for frame synchronization
    DBG("Creating Synchronization Objects...");
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChain->getNumImages(), VK_NULL_HANDLE);
    
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) == VK_SUCCESS &&
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) == VK_SUCCESS &&
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) == VK_SUCCESS)
            DBG("Created Synchronization Objects for frame " << i << "!");
        else
            DBG("Failed to Create Synchronization Objects for frame " << i << "!");
    }
}

void VulkanInstance::submitCommandBuffer(int i)
{
    vkResetCommandBuffer(commandBuffers[i], 0);
    
    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
        DBG("Failed to begin recording command buffer!");
    
    vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);
    
    // Beginning the render pass
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChain->getFrameBuffer(i);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->getExtent();
    
    // Here, you'll typically set clear values for color and depth attachments.
    std::array<VkClearValue, 1> clearValues = {}; // Only 1 clear value as we're not using depth or stencil for now
    clearValues[0].color = {{clearColor.getFloatRed(), clearColor.getFloatGreen(), clearColor.getFloatGreen(), clearColor.getFloatAlpha()}};
    //DBG(cos(speed * frames));
    
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    // Binding the default graphics pipeline
    vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, defaultPipeline->getInternal());
    
    renderComponents(commandBuffers[i]);
    
    vkCmdEndRenderPass(commandBuffers[i]);
    
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        DBG("Failed to Edit Command Buffer " << i << "!");
}

void VulkanInstance::checkForResize()
{
    if (!windowResized.load())
        return;
    if (width.load() <= 0 || height.load() <= 0)
        return;
    
    windowResized.store(false);
    createSwapChain();
}

void VulkanInstance::execute()
{
    //DBG("Rendering: " << totalFrames);
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain->getInternal(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
        return;

    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        DBG("failed to acquire swap chain image!");
    
    // Check if a previous frame is using this image (i.e. there is its fence being waited on)
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
    
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    
    submitCommandBuffer(imageIndex);
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
        DBG("failed to submit draw command buffer!");
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = {swapChain->getInternal()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    
    if (vkQueuePresentKHR(presentQueue, &presentInfo) == VK_ERROR_OUT_OF_DATE_KHR)
        createSwapChain();
    
    totalFrames++;
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanInstance::release()
{
    // Proper cleanup. Destroy objects in reverse order of creation.
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }
    for (size_t i = 0; i < commandBuffers.size(); i++)
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffers[i]);
    
    swapChain.reset();
    
    vkDestroyCommandPool(device, commandPool, nullptr);
    
    defaultPipeline.reset();
    vkDestroyRenderPass(device, renderPass, nullptr);
    
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

} // jvk::core
