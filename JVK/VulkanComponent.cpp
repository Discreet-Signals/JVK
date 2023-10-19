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

void VulkanComponent::sortChildren()
{
    std::sort(children.begin(), children.end(), [](VulkanComponent* a, VulkanComponent* b) { return a->getOrder() < b->getOrder(); });
}

void VulkanComponent::addChildComponent(VulkanComponent* child, bool sort_children)
{
    if (dynamic_cast<VulkanRenderer*>(child))
    {
        DBG("jdk::VulkanComponent - Cannot add VulkanRenderer as child component!");
        return;
    }
    if (child->getPipeline() == VK_NULL_HANDLE)
    {
        DBG("jdk::VulkanComponent - Child must have valid pipeline before adding!");
        return;
    }
    for (VulkanComponent* c : children)
        if (child == c)
            return;
    children.push_back(child);
    child->parent = this;
    child->setRenderer(renderer);
    if (sort_children)
        sortChildren();
}

void VulkanComponent::addChildComponentWithPipeline(VulkanComponent* child, VkPipeline child_pipeline, bool sort_children)
{
    child->setPipeline(child_pipeline);
    addChildComponent(child);
}
void VulkanComponent::addChildComponentWithDefaultPipeline(VulkanComponent* child, bool sort_children)
{
    child->setPipeline(pipeline);
    addChildComponent(child);
}

void VulkanComponent::setRenderer(VulkanRenderer* new_renderer)
{
    if (new_renderer == nullptr)
        return;
    
    if (renderer != nullptr)
        removedFromRenderer(*renderer);
    
    renderer = new_renderer;
    addedToRenderer(*renderer);
    
    for (VulkanComponent* child : children)
        child->setRenderer(new_renderer);
}

void VulkanComponent::renderInternal(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getPipeline());
    
    VkViewport viewport = {};
    viewport.x = bounds.getX();
    viewport.y = bounds.getY();
    viewport.width = bounds.getWidth();
    viewport.height = bounds.getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.offset = {static_cast<int32_t>(viewport.x), static_cast<int32_t>(viewport.y)};
    scissor.extent = {static_cast<uint32_t>(viewport.width), static_cast<uint32_t>(viewport.height)};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    
    render(commandBuffer);
}

void VulkanComponent::renderChildren(VkCommandBuffer& commandBuffer)
{
    for (VulkanComponent* child : children)
    {
        child->renderInternal(commandBuffer);
        child->renderChildren(commandBuffer);
    }
}

VulkanRenderer::VulkanRenderer()
{
    renderer = this;
    initializeVulkan();
    setPipeline(getDefaultPipeline());
    startTimerHz(60);
}

} // jvk
