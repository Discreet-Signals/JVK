/*
  ==============================================================================

    Waveform.h
    Created: 19 Oct 2023 2:03:14pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include "../../../JVK/JVK.h"
#define SIZE 1024

class WaveformDemo : public jvk::ShaderComponent
{
public:
    WaveformDemo() :
        ShaderComponent(jvk::Shaders::ShaderGroup()
                        .addShader(VK_SHADER_STAGE_VERTEX_BIT,
                                   jvk::Shaders::DefaultShaders::BasicShader::basic_vert_spv,
                                   jvk::Shaders::DefaultShaders::BasicShader::basic_vert_spvSize)
                        .addShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                                   BinaryData::Waveform_frag_spv,
                                   BinaryData::Waveform_frag_spvSize), SIZE),
        fifo(2 * SIZE, 0.0f)
    { }
    ~WaveformDemo() { }
    
    void pushAudio(const float* data, int num)
    {
        fifo.push_num(data, num);
        
        while (fifo.size() > 512)
        {
            float peak = 0.0f;
            for (int i = 0; i < 512; i++)
            {
                float s = abs(fifo.pop());
                if (s > peak)
                    peak = s;
            }
            // Could be in preProcess,
            // but since only one value at a time,
            // dealing with pointers and such, not worth it...
            peak = (juce::Decibels::gainToDecibels(peak) + 54.0) / 60.0;
            pushData(&peak, 1);
        }
    }
private:
    jvk::FIFO<float> fifo;

    void preProcessData(float *data, int num) override
    {
        
    }
};
