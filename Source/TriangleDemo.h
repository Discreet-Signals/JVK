/*
  ==============================================================================

    TriangleDemo.h
    Created: 11 Oct 2023 6:16:47pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "JVK/JVK.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

class TriangleDemo : public jvk::VulkanComponent
{
public:
    TriangleDemo() { }
    ~TriangleDemo() { }
private:
    void addedToRenderer(const jvk::VulkanRenderer& renderer) override
    {
        std::vector<Vertex> vertices
        {
            {{ 0.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},// Top vertex
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// Bottom left vertex
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} // Bottom right vertex
        };
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
        if (vkCreateBuffer(renderer.getDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        {
            DBG("failed to create vertex buffer!");
            return;
        }
        
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(renderer.getDevice(), vertexBuffer, &memRequirements);
        
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = jvk::core::findMemoryType(renderer.getPhysicalDevice(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        
        if (vkAllocateMemory(renderer.getDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        {
            DBG("failed to allocate vertex buffer memory!");
            return;
        }
        
        vkBindBufferMemory(renderer.getDevice(), vertexBuffer, vertexBufferMemory, 0);
        
        void* data;
        vkMapMemory(renderer.getDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(renderer.getDevice(), vertexBufferMemory);
    }
    void removedFromRenderer(const jvk::VulkanRenderer& renderer) override
    {
        vkDestroyBuffer(renderer.getDevice(), vertexBuffer, nullptr);
        vkFreeMemory(renderer.getDevice(), vertexBufferMemory, nullptr);
    }
    void render(VkCommandBuffer &commandBuffer) override
    {
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);
        
        // Issue a draw call
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }
    
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
};
