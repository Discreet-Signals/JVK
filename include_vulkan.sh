#!/bin/bash

# Check for at least one argument (project directory)
if [ "$#" -lt 1 ]; then
  echo "Usage: $0 path/to/project_directory [optional_vulkan_sdk_path]"
  exit 1
fi

# Source the shell profile to load environment variables
if [ -f "$HOME/.bash_profile" ]; then
  source "$HOME/.bash_profile"
elif [ -f "$HOME/.zshrc" ]; then
  source "$HOME/.zshrc"
fi

# Set local VULKAN_SDK variable
if [ -n "$2" ]; then
  VULKAN_SDK=$2
  if [ ! -d "${VULKAN_SDK}/include/vulkan" ]; then
    echo "Error: Provided Vulkan SDK path is not valid."
    exit 51
  fi
else
  VULKAN_SDK=${VULKAN_SDK}
  if [ -z "${VULKAN_SDK}" ]; then
    echo "Error: VULKAN_SDK environment variable is not set."
    exit 52
  fi
  if [ ! -d "${VULKAN_SDK}/include/vulkan" ]; then
    echo "Error: VULKAN_SDK environment variable is not valid."
    exit 53
  fi
fi

# Project Paths
JVK_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH=$1
JUCER_FILE=$(find "${PROJECT_PATH}" -maxdepth 1 -name "*.jucer" -print -quit)

# Ensure the .jucer file exists
if [ ! -f "${JUCER_FILE}" ]; then
  echo "Error: .jucer file not found in ${PROJECT_PATH}"
  exit 1
fi

PROJECT_NAME=$(basename "${JUCER_FILE}" .jucer)
echo "Adding Vulkan paths to ${PROJECT_NAME}..."

# Paths to add (modify these as needed)
EXTRA_DEFS="VK_USE_PLATFORM_MACOS_MVK"
HEADER_SEARCH_PATHS="/Users/gavin/VulkanSDK/1.3.261.1/macOS/include"
LIBRARY_SEARCH_PATHS="/Users/gavin/VulkanSDK/1.3.261.1/MoltenVK/dylib/macOS"
FRAMEWORK_SEARCH_PATHS="/Users/gavin/VulkanSDK/1.3.261.1/macOS/Frameworks"
EMBEDDED_FRAMEWORKS="/Users/gavin/VulkanSDK/1.3.261.1/macOS/Frameworks/vulkan.framework"
EXTERNAL_LIBRARIES="MoltenVK"
LINKER_FLAGS="-rpath @loader_path/../Frameworks"
POST_BUILD_SHELL_SCRIPT='if [[ ! \&quot;$FULL_PRODUCT_NAME\&quot; == *.a ]]; then\&#10;    cp \&quot;/Users/gavin/VulkanSDK/1.3.261.1/MoltenVK/dylib/macOS/libMoltenVK.dylib\&quot; \&quot;$CONFIGURATION_BUILD_DIR/$FULL_PRODUCT_NAME/Contents/Frameworks\&quot;\&#10;fi'

# Function to add module to the Projucer project
add_module_to_projucer() {
  local jucer_file=$1
  local module_id=$2

  # Check if the module is already added
  if ! grep -q "<MODULE id=\"${module_id}\"" "${jucer_file}"; then
    sed -i '' '/<\/MODULES>/i\
      <MODULE id="'"${module_id}"'" showAllCode="1" useLocalCopy="0" useGlobalPath="0"/>\
    ' "${jucer_file}"
  fi
  if ! grep -q "<MODULEPATH id=\"${module_id}\"" "${jucer_file}"; then
    sed -i '' '/<\/MODULEPATHS>/i\
      <MODULEPATH id="'"JVK"'" path="'"${JVK_PATH}"'"/>\
    ' "${jucer_file}"
  fi
}

# Function to add search paths and linker flags to the Projucer project
configure_projucer() {
  local jucer_file=$1

  # Function to append or create new fields before the closing > of XCODE_MAC
  append_or_create() {
    local field=$1
    local value=$2
    local section=$3

    if [[ $section == "CONFIGURATION" ]]; then
      # Handle CONFIGURATION section, which ends with '/>'
      if ! grep -q "$field=" "${jucer_file}"; then
        sed -i '' "s|<CONFIGURATION |<CONFIGURATION $field=\"$value\" |" "$jucer_file"
      fi
    else
      # Handle other sections, which end with '>'
      if ! grep -q "$field=" "${jucer_file}"; then
        sed -i '' "/<$section / s|>| $field=\"$value\">|" "$jucer_file"
      fi
    fi
  }

  # Append or create fields in the XCODE_MAC section
  append_or_create "extraDefs" "$EXTRA_DEFS" "XCODE_MAC"
  append_or_create "frameworkSearchPaths" "$FRAMEWORK_SEARCH_PATHS" "XCODE_MAC"
  append_or_create "embeddedFrameworks" "$EMBEDDED_FRAMEWORKS" "XCODE_MAC"
  append_or_create "externalLibraries" "$EXTERNAL_LIBRARIES" "XCODE_MAC"
  append_or_create "extraLinkerFlags" "$LINKER_FLAGS" "XCODE_MAC"
  append_or_create "postbuildCommand" "$POST_BUILD_SHELL_SCRIPT" "XCODE_MAC"
  append_or_create "headerPath" "$HEADER_SEARCH_PATHS" "CONFIGURATION"
  append_or_create "libraryPath" "$LIBRARY_SEARCH_PATHS" "CONFIGURATION"
}
# Add the Vulkan module to the Projucer project
add_module_to_projucer "${JUCER_FILE}" "JVK"

# Configure the Projucer project with search paths and linker flags
configure_projucer "${JUCER_FILE}"

echo "ProJucer project configured: ${JUCER_FILE}"
