cmake_minimum_required(VERSION 3.15...3.31)

# Top-level CMakeLists.txt for building all plugins at once
# This is optional - each plugin can also be built independently from its own directory
# by navigating to plugins/<plugin_name> and running cmake there.

project(slPlugins VERSION 1.1.0)

set (CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
set (CMAKE_OSX_ARCHITECTURES arm64 x86_64)
set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(GLOBAL PROPERTY XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO)

add_subdirectory(modules)
add_subdirectory(plugins)
