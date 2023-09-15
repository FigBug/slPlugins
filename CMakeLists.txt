cmake_minimum_required(VERSION 3.15)

project(slPlugins VERSION 1.1.0)

set (CMAKE_OSX_DEPLOYMENT_TARGET 10.9)

add_subdirectory(modules)
add_subdirectory(plugins)
