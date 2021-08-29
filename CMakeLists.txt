cmake_minimum_required(VERSION 3.10)

project(meteor_postprocess
	VERSION 1.0
	DESCRIPTION "LRPT image postprocessor"
	LANGUAGES C)

add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")

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
	gui/gui.c gui/gui.h
	gui/composite_preview.c
	gui/menu.c

	effects/enhancements.c effects/enhancements.h

	palettes/palettes.h
	palettes/hvc.c

	channel.c channel.h
	composite.c composite.h
	utils.h
)

set(COMMON_INC_DIRS
	${PROJECT_SOURCE_DIR}
)

# Main executable target
add_executable(meteor_postprocess main.c ${COMMON_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})
target_include_directories(meteor_postprocess PUBLIC ${COMMON_INC_DIRS})
target_link_libraries(meteor_postprocess PUBLIC ${GTK_LIBRARIES} pthread)

# Add links to ncurses if enabled
# if (ENABLE_TUI AND NCURSES_LIBRARY)
# target_link_libraries(meteor_demod PUBLIC ${NCURSES_LIBRARY})
# endif()

install(TARGETS meteor_postprocess DESTINATION bin)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

