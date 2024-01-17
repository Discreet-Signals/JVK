/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION

 ID:               JVK
 vendor:           Discreet Signals
 version:          1.0
 name:             jvk
 description:      Vulkan Integration for JUCE
 website:          https://github.com/Discreet-Signals/JVK
 dependencies:     juce_core
 
 END_JUCE_MODULE_DECLARATION

 *******************************************************************************/

#pragma once
#include <juce_core/juce_core.h>

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
#include "Core/Windows/VulkanHWNDComponent.h"
#else
#error "JVK: Unsupported Platform!"
#endif

// ECS
#include "ECS/ECS.h"
#include "ECS/Objects/Camera.h"

#include "Components/Components.h"
#include "FIFO.h"
