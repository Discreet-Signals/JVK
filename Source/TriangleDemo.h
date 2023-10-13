/*
  ==============================================================================

    TriangleDemo.h
    Created: 11 Oct 2023 6:16:47pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "JVK/VulkanComponent.h"

class TriangleDemo : public jvk::VulkanComponent
{
public:
    TriangleDemo()
    {
        std::vector<jvk::Vertex> vertices
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
        
        if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        {
            DBG("failed to create vertex buffer!");
            return;
        }
        
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);
        
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = jvk::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        
        if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        {
            DBG("failed to allocate vertex buffer memory!");
            return;
        }
        
        vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
        
        void* data;
        vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(device, vertexBufferMemory);
    }
    
    ~TriangleDemo()
    {
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
    }
private:
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
