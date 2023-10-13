/*
  ==============================================================================

    VulkanInstance.cpp
    Created: 11 Oct 2023 1:53:22pm
    Author:  Gavin

  ==============================================================================
*/

#include "VulkanInstance.h"
#include <fstream>

namespace jvk
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
    swapChain = std::make_unique<SwapChain>(info);
    
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

std::vector<char> VulkanInstance::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    
    return buffer;
}

VkShaderModule VulkanInstance::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
    
    return shaderModule;
}

void VulkanInstance::createGraphicsPipeline()
{
    // 1. Load and compile shaders
    auto vertShaderCode = readFile("/Users/gavin/Documents/GitHub/JuceVulkan/Source/Shaders/basic.vert.spv");
    auto fragShaderCode = readFile("/Users/gavin/Documents/GitHub/JuceVulkan/Source/Shaders/basic.frag.spv");
    
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
    
    // 2. Set up shader stage info
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    
    // 3. Vertex input
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(glm::vec3) + sizeof(glm::vec4); // vec3 for position + vec4 for color.
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
    
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0; // Matches the position's location in the shader
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // Format for vec3
    attributeDescriptions[0].offset = 0;
    
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1; // Matches the color's location in the shader
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT; // Format for vec4
    attributeDescriptions[1].offset = sizeof(glm::vec3); // Color comes after the position
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    
    // 4. Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    // 5. Viewports and scissors
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChain->getWidth();
    viewport.height = (float) swapChain->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    scissor.offset = {0, 0};
    scissor.extent = swapChain->getExtent();
    
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    
    // 6. Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    
    // 7. Multisampling (disabled for now)
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;
    
    // 8. Depth and stencil testing (assuming you have this setup)
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    
    // 9. Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    
    // 10. Pipeline layout creation (we don't use any descriptor sets for now)
    DBG("Creating Pipeline Layout...");
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS)
        DBG("Created Pipeline Layout!");
    else
        DBG("Failed to Create Pipeline Layout!");
    
    // 11. Bringing it all together for the graphics pipeline creation
    DBG("Creating Graphics Pipeline...");
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    pipelineInfo.pDynamicState = &dynamicState;
    
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) == VK_SUCCESS)
        DBG("Created Graphics Pipeline!");
    else
        DBG("Failed to Create Graphics Pipeline!");
    
    // Clean up the temporary shader modules
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
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
    DBG("Creating Command Buffers...");
    commandBuffers.resize(swapChain->getNumImages());
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();
    
    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        DBG("Failed to allocate command buffers!");
    
    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        DBG("Creating Command Buffer " << i << "...");
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional
        
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            DBG("Failed to begin recording command buffer!");
        
        // Beginning the render pass
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(i);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getExtent();
        
        // Here, you'll typically set clear values for color and depth attachments.
        std::array<VkClearValue, 1> clearValues = {}; // Only 1 clear value as we're not using depth or stencil for now
        clearValues[0].color = {{0.0f, 0.0f, 1.0f, 1.0f}}; // Clearing to blue color as an example
        
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        // Binding the graphics pipeline
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        
        // TODO: Bind your vertex buffers, index buffers, descriptor sets, etc.
        // and then issue draw calls like vkCmdDraw or vkCmdDrawIndexed.
        
        vkCmdEndRenderPass(commandBuffers[i]);
        
        if (vkEndCommandBuffer(commandBuffers[i]) == VK_SUCCESS)
            DBG("Created Command Buffer " << i << "!");
        else
            DBG("Failed to Create Command Buffer " << i << "!");
    }
    
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
    
    // Binding the graphics pipeline
    vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    
    render(commandBuffers[i]);
    vkCmdEndRenderPass(commandBuffers[i]);
    
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        DBG("Failed to Edit Command Buffer " << i << "!");
}

void VulkanInstance::checkForResize()
{
    if (!windowResized.load())
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
    
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result != VK_SUCCESS)
        DBG("failed to present swap chain image!");
    
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
    vkDestroyCommandPool(device, commandPool, nullptr);
    
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

} // jvk
