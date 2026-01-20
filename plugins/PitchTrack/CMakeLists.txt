cmake_minimum_required(VERSION 3.24.0)

# Plugin settings
set(PLUGIN_NAME PitchTrack)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION PLUGIN_VERSION)
string(STRIP ${PLUGIN_VERSION} PLUGIN_VERSION)
set(BUNDLE_ID com.socalabs.PitchTrack)
set(PLUGIN_CODE SLpt)
set(AU_ID PitchTrackAU)
set(LV2_URI https://socalabs.com/pitchtrack/)

# Platform-specific settings
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Include shared module setup
include(${CMAKE_CURRENT_LIST_DIR}/../../modules/setup_modules.cmake)

project(${PLUGIN_NAME} VERSION ${PLUGIN_VERSION})

# Plugin formats
if(WIN32)
    set(FORMATS Standalone VST VST3 LV2)
else()
    set(FORMATS Standalone VST VST3 AU LV2)
endif()

juce_add_plugin(${PLUGIN_NAME}
    PRODUCT_NAME "PitchTrack"
    VERSION ${PLUGIN_VERSION}
    COMPANY_NAME "SocaLabs"
    COMPANY_WEBSITE "www.socalabs.com"
    COMPANY_EMAIL "info@socalabs.com"
    BUNDLE_ID ${BUNDLE_ID}
    FORMATS ${FORMATS}
    PLUGIN_MANUFACTURER_CODE Soca
    PLUGIN_CODE ${PLUGIN_CODE}
    AU_EXPORT_PREFIX ${AU_ID}
    AU_MAIN_TYPE kAudioUnitType_Effect
    VST2_CATEGORY kPlugCategEffect
    LV2URI ${LV2_URI}
    NEEDS_MIDI_INPUT ON
)

clap_juce_extensions_plugin(TARGET ${PLUGIN_NAME}
    CLAP_ID "${BUNDLE_ID}"
    CLAP_FEATURES audio-effect analyzer)

# Auto-discover source files
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h
)

target_sources(${PLUGIN_NAME} PRIVATE ${source_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX Source FILES ${source_files})

juce_generate_juce_header(${PLUGIN_NAME})

# Extra include directories for Q library
target_include_directories(${PLUGIN_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/Q/q_lib/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/infra/include
)

target_compile_definitions(${PLUGIN_NAME}
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_JACK=0
        JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
        DONT_SET_USING_JUCE_NAMESPACE=1
)

set_target_properties(${PLUGIN_NAME} PROPERTIES
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

target_link_libraries(${PLUGIN_NAME}
    PRIVATE
        gin
        gin_dsp
        gin_simd
        gin_graphics
        gin_gui
        gin_plugin
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_cryptography
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Platform-specific settings
if(APPLE)
    foreach(t ${FORMATS} "Assets" "All" "")
        set(tgt ${PLUGIN_NAME})
        if(NOT t STREQUAL "")
            set(tgt ${tgt}_${t})
        endif()
        if(TARGET ${tgt})
            set_target_properties(${tgt} PROPERTIES
                XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO
                XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
            )
        endif()
    endforeach()
endif()
