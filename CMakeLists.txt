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

# Skip install rules everywhere except Linux, where we generate per-plugin .deb
# from staged trees in Installer/build.sh — we set up per-plugin install rules
# below so cmake --install can produce the artefact tree we then package.
if (NOT (UNIX AND NOT APPLE))
	set (CMAKE_SKIP_INSTALL_RULES YES)
endif()

# Per-plugin Linux install rules. Installer/build.sh runs `cmake --install`
# with a per-plugin component to extract just one plugin's artefacts at a time.
if (UNIX AND NOT APPLE)
	file (READ "${CMAKE_CURRENT_SOURCE_DIR}/ci/pluginlist.txt" _pl)
	string (REGEX REPLACE "\n" ";" _pl "${_pl}")
	foreach (_p IN LISTS _pl)
		string (STRIP "${_p}" _p)
		if (NOT _p)
			continue()
		endif()
		set (_a "${CMAKE_BINARY_DIR}/plugins/${_p}/${_p}_artefacts/$<CONFIG>")

		install (FILES "${_a}/VST/lib${_p}.so"
				 DESTINATION "lib/vst" RENAME "${_p}.so" COMPONENT "${_p}")
		install (DIRECTORY "${_a}/VST3/${_p}.vst3"
				 DESTINATION "lib/vst3" COMPONENT "${_p}" USE_SOURCE_PERMISSIONS)
		install (DIRECTORY "${_a}/LV2/${_p}.lv2"
				 DESTINATION "lib/lv2" COMPONENT "${_p}" USE_SOURCE_PERMISSIONS)
		install (FILES "${_a}/CLAP/${_p}.clap"
				 DESTINATION "lib/clap" COMPONENT "${_p}")

		# Factory preset XMLs for the 9 plugins that bundle them.
		file (GLOB _xmls "${CMAKE_CURRENT_SOURCE_DIR}/plugins/${_p}/Resources/*.xml")
		if (_xmls)
			install (FILES ${_xmls}
					 DESTINATION "share/SocaLabs/${_p}/Presets"
					 COMPONENT "${_p}")
		endif()
	endforeach()
endif()
