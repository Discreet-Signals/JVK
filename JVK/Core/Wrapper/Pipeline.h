/*
  ==============================================================================

    Pipeline.h
    Created: 18 Oct 2023 1:11:41pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "Shader.h"

namespace jvk
{

class Pipeline
{
public:
    template <typename SG>
    Pipeline(VkDevice vk_device, VkRenderPass render_pass, SG&& shader_group) :
        device(vk_device),
        renderPass(render_pass),
        shaderGroup(std::forward<SG>(shader_group))
    {
        create();
    }
    ~Pipeline();
    
    VkPipeline getInternal() const { return graphicsPipeline; }
private:
    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    void create();
    
    VkDevice device;
    VkRenderPass renderPass;
    Shaders::ShaderGroup shaderGroup;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

}
