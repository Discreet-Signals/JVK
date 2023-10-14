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
#include "core/Wrapper/Wrapper.h"
#include "core/VulkanInstance.h"
#if JUCE_MAC
#include "core/MacOS/NSViewGenerator.h"
#include "core/MacOS/VulkanNSViewComponent.h"
#endif
#if JUCE_WINDOWS
// TODO: WINDOWS SUPPRT
#endif

// ECS
#include "ECS/ECS.h"
#include "ECS/Objects/Camera.h"

#include "VulkanComponent.h"

