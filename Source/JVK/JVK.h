/*
  ==============================================================================

    JVK.h
    Created: 14 Oct 2023 4:41:43pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once

// Dependencies
#include <JuceHeader.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

// Core
#include "Core/Wrapper/Wrapper.h"
#include "Core/VulkanInstance.h"
#if JUCE_MAC
#include "Core/MacOS/NSViewGenerator.h"
#include "Core/MacOS/VulkanNSViewComponent.h"
#elif JUCE_WINDOWS
// TODO: WINDOWS SUPPRT
#else
#error "JVK: Unsupported Platform!"
#endif

// ECS
#include "ECS/ECS.h"
#include "ECS/Objects/Camera.h"

#include "VulkanComponent.h"

