# Shared module setup for slPlugins
# Include this file from each plugin's CMakeLists.txt to set up dependencies

cmake_minimum_required(VERSION 3.15)

# Get the path to the modules directory (parent of this file)
get_filename_component(MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# Add JUCE
add_subdirectory(${MODULES_DIR}/juce ${CMAKE_BINARY_DIR}/juce EXCLUDE_FROM_ALL)

# Set VST2 SDK path
juce_set_vst2_sdk_path(${MODULES_DIR}/plugin_sdk/vstsdk2.4)

# Add JUCE modules
juce_add_module(${MODULES_DIR}/dRowAudio/module/dRowAudio)
juce_add_module(${MODULES_DIR}/gin/modules/gin)
juce_add_module(${MODULES_DIR}/gin/modules/gin_dsp)
juce_add_module(${MODULES_DIR}/gin/modules/gin_simd)
juce_add_module(${MODULES_DIR}/gin/modules/gin_gui)
juce_add_module(${MODULES_DIR}/gin/modules/gin_metadata)
juce_add_module(${MODULES_DIR}/gin/modules/gin_network)
juce_add_module(${MODULES_DIR}/gin/modules/gin_plugin)
juce_add_module(${MODULES_DIR}/gin/modules/gin_graphics)
juce_add_module(${MODULES_DIR}/pitch_detector)
juce_add_module(${MODULES_DIR}/audio_fft)
