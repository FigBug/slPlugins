# Shared module setup for slPlugins
# Include this file from each plugin's CMakeLists.txt to set up dependencies

cmake_minimum_required(VERSION 3.15)

# Get the path to the modules directory (parent of this file)
get_filename_component(MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# Add JUCE only if not already added
if(NOT TARGET juce::juce_core)
    set (JUCE_MODULES_ONLY OFF)
    set (JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
    set (JUCE_BUILD_EXTRAS OFF)
    set (JUCE_BUILD_EXAMPLES OFF)
    add_subdirectory(${MODULES_DIR}/juce ${CMAKE_BINARY_DIR}/juce EXCLUDE_FROM_ALL)
endif()

# Set VST2 SDK path only if not already set
if(NOT TARGET juce_vst2_sdk)
    juce_set_vst2_sdk_path(${MODULES_DIR}/plugin_sdk/vstsdk2.4)
endif()

# Add JUCE modules only if not already added
if(NOT TARGET dRowAudio)
    juce_add_module(${MODULES_DIR}/dRowAudio/module/dRowAudio)
endif()

if(NOT TARGET gin)
    juce_add_module(${MODULES_DIR}/gin/modules/gin)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_dsp)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_simd)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_gui)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_metadata)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_network)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_plugin)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_graphics)
    juce_add_module(${MODULES_DIR}/gin/modules/gin_standaloneplugin)
endif()

if(NOT TARGET pitch_detector)
    juce_add_module(${MODULES_DIR}/pitch_detector)
endif()

if(NOT TARGET audio_fft)
    juce_add_module(${MODULES_DIR}/audio_fft)
endif()
