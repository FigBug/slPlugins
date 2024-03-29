# This file was generated by FRUT's Jucer2CMake from "Limiter.jucer"

cmake_minimum_required(VERSION 3.15)

project("Limiter")

juce_add_plugin(Limiter
  VERSION "1.1.0"
  BUNDLE_ID "com.socalabs.Limiter"
  COMPANY_NAME "SocaLabs"
  COMPANY_COPYRIGHT "SocaLabs"
  COMPANY_WEBSITE "www.socalabs.com"
  COMPANY_EMAIL "info@socalabs.com"

  FORMATS
    "VST3"
    "AU"
    "Standalone"
    "VST"
    "LV2"
  PLUGIN_NAME "Limiter"
  DESCRIPTION "Limiter"
  PLUGIN_MANUFACTURER_CODE "Soca"
  PLUGIN_CODE "SLlm"
  AAX_IDENTIFIER "com.socalabs.Limiter"
  AU_EXPORT_PREFIX "LimiterAU"
  AU_MAIN_TYPE "kAudioUnitType_Effect"
  VST2_CATEGORY "kPlugCategEffect"
  LV2URI "https://socalabs.com/limiter/"
)

juce_generate_juce_header(Limiter)

target_compile_definitions(Limiter
  PUBLIC
    JUCE_QUICKTIME=disabled
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_MODAL_LOOPS_PERMITTED=1
    JUCE_JACK=1
    JUCE_REPORT_APP_USAGE=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    
    JucePlugin_MaxNumInputChannels=2
    JucePlugin_MaxNumOutputChannels=2
    JucePlugin_PreferredChannelConfigurations=2,2,1,1
)

target_sources(Limiter
  PRIVATE
    "Source/PluginEditor.cpp"
    "Source/PluginProcessor.cpp"
)

set_target_properties(Limiter PROPERTIES
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

target_link_libraries(Limiter
  PRIVATE
    dRowAudio
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

if (APPLE)
	set_target_properties("juce_vst3_helper" PROPERTIES XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO)
  foreach(t ${FORMATS} "Assets" "All" "")
		set_target_properties(${tgt} PROPERTIES
			XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO
			XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES")
  endforeach()
endif()     