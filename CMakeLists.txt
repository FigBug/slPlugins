cmake_minimum_required(VERSION 3.15)

project(slPlugins VERSION 1.1.0)

set (CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
set (CMAKE_OSX_ARCHITECTURES arm64 x86_64)
set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(GLOBAL PROPERTY XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO)

add_subdirectory(modules)
add_subdirectory(plugins)
