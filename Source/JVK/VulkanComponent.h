/*
  ==============================================================================

    VulkanComponent.h
    Created: 8 Oct 2023 4:35:57pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#if JUCE_MAC
#include "Core/MacOS/VulkanNSViewComponent.h"
#elif JUCE_WINDOWS
#include "Core/Windows/VulkanHWNDComponent.h"
#endif

namespace jvk
{

class VulkanRenderer;

class VulkanComponent
{
public:
    VulkanComponent() { }
    virtual ~VulkanComponent() { if (renderer != nullptr) removedFromRenderer(*renderer); }
    
    virtual void addedToRenderer(const jvk::VulkanRenderer& renderer) { };
    virtual void removedFromRenderer(const jvk::VulkanRenderer& renderer) { };
    virtual void render(VkCommandBuffer& commandBuffer) { };
    virtual void resized() { };
    
    void setPipeline(VkPipeline new_pipeline) { pipeline = new_pipeline; }
    VkPipeline getPipeline() const { return pipeline; }
    
    void setBounds(juce::Rectangle<float> new_bounds) { bounds = new_bounds; resized(); }
    void setBounds(float x, float y, float width, float height) { setBounds({x,y,width,height}); }
    juce::Rectangle<float> getBounds() const { return bounds; }
    
    void setOrder(int new_order) { order = new_order; }
    int getOrder() const { return order; }
    
    void sortChildren();
    void addChildComponent(VulkanComponent* child, bool sort_children = true);
    void addChildComponentWithPipeline(VulkanComponent* child, VkPipeline pipeline, bool sort_children = true);
    void addChildComponentWithDefaultPipeline(VulkanComponent* child, bool sort_children = true);
    const std::vector<VulkanComponent*> getChildren() const { return children; }
    VulkanRenderer* getRenderer() const { return renderer; }
    VulkanComponent* getParent() { return parent; }
    
private:
    friend class VulkanRenderer;
    void setRenderer(VulkanRenderer* renderer);
    void renderInternal(VkCommandBuffer& commandBuffer);
    void renderChildren(VkCommandBuffer& commandBuffer);
    VulkanRenderer* renderer { nullptr };
    VulkanComponent* parent { nullptr };
    std::vector<VulkanComponent*> children { };
    juce::Rectangle<float> bounds { 0,0,0,0 };
    VkPipeline pipeline { VK_NULL_HANDLE };
    int order { 0 };
    
};

#if JUCE_MAC
using OSWindowComponent = core::macos::VulkanNSViewComponent;
#elif JUCE_WINDOWS
using OSWindowComponent = core::windows::VulkanHWMDComponent,
#endif
class VulkanRenderer :
    private VulkanComponent,
    public OSWindowComponent,
    public juce::Timer
{
public:
    VulkanRenderer();
    virtual ~VulkanRenderer() { };
    
    VkPipeline getDefaultPipeline() const { return graphicsPipeline; }
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
    VkDevice getDevice() const { return device; }
    
    void setBounds(juce::Rectangle<int> new_bounds) { OSWindowComponent::setBounds(new_bounds); }
    void setBounds(int x, int y, int width, int height) { OSWindowComponent::setBounds({x,y,width,height}); }
    juce::Rectangle<int> getBounds() const { return OSWindowComponent::getBounds(); }
    
    void sortChildren() { VulkanComponent::sortChildren(); }
    void addChildComponent(VulkanComponent* child, bool sort_children = true) { VulkanComponent::addChildComponent(child, sort_children); }
    void addChildComponentWithPipeline(VulkanComponent* child, VkPipeline pipeline, bool sort_children = true) { VulkanComponent::addChildComponentWithPipeline(child, pipeline, sort_children); }
    void addChildComponentWithDefaultPipeline(VulkanComponent* child, bool sort_children = true) { VulkanComponent::addChildComponentWithDefaultPipeline(child, sort_children); }
    const std::vector<VulkanComponent*> getChildren() const { return children; }
protected:
    virtual void render(VkCommandBuffer& commandBuffer) override { };
private:
    void renderComponents(VkCommandBuffer& commandBuffer) override
    {
        renderInternal(commandBuffer);
        const std::vector<VulkanComponent*> components = VulkanComponent::getChildren();
        for (VulkanComponent* comp : components)
        {
            comp->renderInternal(commandBuffer);
            comp->renderChildren(commandBuffer);
        }
    };
    void timerCallback() override { checkForResize(); execute(); }
};

} // jvk
