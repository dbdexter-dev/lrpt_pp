cmake_minimum_required(VERSION 3.10)

option(ENABLE_GUI "Enable GTK+3 GUI" ON)

project(lrpt_pp
	VERSION 0.1
	DESCRIPTION "LRPT image postprocessor"
	LANGUAGES C)

add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")

if (ENABLE_GUI)
	# GTK+ config
	include(FindPkgConfig)
	pkg_check_modules(GTK REQUIRED "gtk+-3.0")
	include_directories(${GTK_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
	add_definitions(${GTK3_CFLAGS_OTHER})
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic")

	# Custom commands to embed glib resources in executable
	find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
	set(GRESOURCE_XML resources.xml)
	set(GRESOURCE_C resources.c)
	add_custom_command(
		OUTPUT ${GRESOURCE_C}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${GLIB_COMPILE_RESOURCES}
		ARGS
			--target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
			--generate-source
			${GRESOURCE_XML}
		VERBATIM
		MAIN_DEPENDENCY ${GRESOURCE_XML}
		DEPENDS glade/ui.glade
	)
	set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} PROPERTIES GENERATED TRUE)

	# Enable GUI compilation
	add_definitions(-DENABLE_GTK)
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -Wextra -Wimplicit-fallthrough")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native -ftree-vectorize")
# ARM architectures need -mfpu=auto in order to enable NEON when available,
# but that option is unrecognized by x86 gcc (and possibly others): only
# add it to the release flags when the compiler's target is arm
# This is not a problem for arm64, as NEON support is mandatory for that arch
execute_process(COMMAND "${CMAKE_C_COMPILER}" "-dumpmachine" COMMAND "grep" "arm" OUTPUT_QUIET RESULT_VARIABLE is_arm)
if (is_arm EQUAL "0")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=native -mfpu=auto")
endif()


set(COMMON_SOURCES
	effects/enhancements.c effects/enhancements.h

	palettes/palettes.h

	image.c image.h
	composite.c composite.h
	utils.c utils.h
)

set(GTK_SOURCES
	gui/gui.c gui/gui.h
	gui/save.c gui/save.h
	gui/composite_preview.c
	gui/menu.c

	${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)

set(COMMON_INC_DIRS
	${PROJECT_SOURCE_DIR}
)

# Main executable target
if (ENABLE_GUI)
	add_executable(lrpt_pp main.c ${COMMON_SOURCES} ${GTK_SOURCES})
	target_include_directories(lrpt_pp PUBLIC ${COMMON_INC_DIRS})
	target_link_libraries(lrpt_pp PUBLIC ${GTK_LIBRARIES} pthread)
else()
	add_executable(lrpt_pp main.c ${COMMON_SOURCES})
	target_include_directories(lrpt_pp PUBLIC ${COMMON_INC_DIRS})
	target_link_libraries(lrpt_pp PUBLIC pthread)
endif()


install(TARGETS lrpt_pp DESTINATION bin)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

