/*
  ==============================================================================

    Shader.h
    Created: 18 Oct 2023 2:55:53pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <fstream>

namespace jvk
{

class Pipeline;

namespace Shaders
{

enum class Bits
{
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

static inline std::vector<char> readFile(const std::string& filename)
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
};

struct Shader
{
    VkShaderStageFlagBits stage;
    std::vector<char> code;
};
class ShaderGroup
{
public:
    ShaderGroup() { }
    
    template<typename S>
    ShaderGroup withShader(S&& shader)
    {
        ShaderGroup newGroup(*this);
        newGroup.shaders.push_back(std::forward<S>(shader));
        return newGroup;
    }
    ShaderGroup withShader(VkShaderStageFlagBits stage, std::vector<char> code)
    {
        ShaderGroup newGroup(*this);
        Shader shader = { stage, code };
        newGroup.shaders.push_back(std::move(shader));
        return newGroup;
    }
    ShaderGroup withShader(VkShaderStageFlagBits stage, const char* binary_data_spv, const int binary_data_spvSize)
    {
        ShaderGroup newGroup(*this);
        Shader shader = { stage, std::vector<char>(binary_data_spv, binary_data_spv + binary_data_spvSize) };
        newGroup.shaders.push_back(std::move(shader));
        return newGroup;
    }
    ShaderGroup withShader(VkShaderStageFlagBits stage, const juce::String& file)
    {
        ShaderGroup newGroup(*this);
        Shader shader = { stage, readFile(file.toStdString()) };
        newGroup.shaders.push_back(std::move(shader));
        return newGroup;
    }
    ShaderGroup withDescriptorSetLayout(VkDescriptorSetLayout layout)
    {
        ShaderGroup newGroup(*this);
        newGroup.descriptorSetLayouts.push_back(layout);
        return newGroup;
    }
    
    template<typename S>
    ShaderGroup& addShader(S&& shader)
    {
        shaders.push_back(std::forward<S>(shader));
        return *this;
    }
    ShaderGroup& addShader(VkShaderStageFlagBits stage, std::vector<char> code)
    {
        Shader shader = { stage, code };
        shaders.push_back(std::move(shader));
        return *this;
    }
    ShaderGroup& addShader(VkShaderStageFlagBits stage, const char* binary_data_spv, const int binary_data_spvSize)
    {
        Shader shader = { stage, std::vector<char>(binary_data_spv, binary_data_spv + binary_data_spvSize) };
        shaders.push_back(std::move(shader));
        return *this;
    }
    ShaderGroup& addShader(VkShaderStageFlagBits stage, const juce::String& file)
    {
        Shader shader = { stage, readFile(file.toStdString()) };
        shaders.push_back(std::move(shader));
        return *this;
    }
    ShaderGroup& addDescriptorSetLayout(VkDescriptorSetLayout layout)
    {
        descriptorSetLayouts.push_back(layout);
        return *this;
    }

private:
    friend class ::jvk::Pipeline;
    std::vector<Shader> shaders;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
};

}} // jvk::Shader
