#pragma once
#include "JVK.h"

// Core
#include "Core/Wrapper/Wrapper.cpp"
#include "Core/VulkanInstance.cpp"
#if JUCE_MAC
#include "Core/MacOS/VulkanNSViewComponent.cpp"
#elif JUCE_WINDOWS
#include "Core/Windows/VulkanHWNDComponent.cpp"
#endif

// Components
#include "Components/Components.cpp"
