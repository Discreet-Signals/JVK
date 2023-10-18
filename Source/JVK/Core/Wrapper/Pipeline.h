/*
  ==============================================================================

    Pipeline.h
    Created: 18 Oct 2023 1:11:41pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace jvk
{

struct Shader
{
    VkShaderStageFlagBits stage;
    std::vector<char> code;
};
struct ShaderStage
{
    juce::String name;
    std::vector<Shader> shaders;
};

class Pipeline
{
public:
    Pipeline(VkDevice vk_device, VkRenderPass render_pass, const ShaderStage& shader_stage);
    ~Pipeline();
    
    VkPipeline getInternal() const { return graphicsPipeline; }
private:
    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    
    VkDevice device;
    Shader shader;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

}
