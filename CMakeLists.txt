cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "")
  # Intel Homebrew CMake on Apple Silicon otherwise produces x86_64-only binaries.
  # Pass -DBITQUEST_UNIVERSAL=ON for a single Intel + Apple Silicon package.
  if(BITQUEST_UNIVERSAL)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
  elseif(NOT CMAKE_OSX_ARCHITECTURES)
    execute_process(
      COMMAND sysctl -n hw.optional.arm64
      OUTPUT_VARIABLE BITQUEST_HOST_ARM64
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET)
    if(BITQUEST_HOST_ARM64 STREQUAL "1")
      set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "")
    endif()
  endif()
endif()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### setup Directories ####
#Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### EXTERNAL DEPENDENCIES ####

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")



#### ENGINE####

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")

file(GLOB_RECURSE SOURCE_FILES engine/src/*.cpp engine/src/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine PUBLIC
  "${CMAKE_SOURCE_DIR}/engine/src"
  "${CMAKE_SOURCE_DIR}/code")
target_include_directories(lib_engine SYSTEM PUBLIC ${B2D_INCS})
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

## Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths" INTERFACE ${SFML_INCS})

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_tile_level_loader/*.cpp engine/lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

## ECM lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_ecm/*.cpp engine/lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)



#### BITQUEST ####
file(GLOB_RECURSE SOURCES code/*.cpp code/*.h)
file(GLOB_RECURSE CMPNTS code/components/*.cpp code/components/*.h)
file(GLOB_RECURSE SCENES code/scenes/*.cpp code/scenes/*.h)
add_executable(BITQUEST ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
target_include_directories(BITQUEST PRIVATE "${CMAKE_SOURCE_DIR}/code")
target_link_libraries(BITQUEST lib_engine)
set(EXECUTABLES ${EXECUTABLES} BITQUEST)

if(APPLE)
  set_target_properties(BITQUEST PROPERTIES
    MACOSX_BUNDLE TRUE
    OUTPUT_NAME "BitQuest"
    MACOSX_BUNDLE_BUNDLE_NAME "BitQuest"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.bitquest.game"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
    MACOSX_BUNDLE_COPYRIGHT "Thomas Young, Mark Barton"
    BUILD_RPATH "@executable_path/../Frameworks;@executable_path"
    INSTALL_RPATH "@executable_path/../Frameworks;@executable_path"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
else()
  set_target_properties(BITQUEST PROPERTIES OUTPUT_NAME "BitQuest")
endif()

add_custom_command(TARGET BITQUEST POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:BITQUEST>/res"
)

set_target_properties(BITQUEST PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:BITQUEST>"
)

if(WIN32)
  foreach(sfml_target sfml-system sfml-window sfml-graphics sfml-audio)
    add_custom_command(TARGET BITQUEST POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${sfml_target}>" "$<TARGET_FILE_DIR:BITQUEST>"
    )
  endforeach()
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64")
    set(BITQUEST_OPENAL_DLL "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/ARM64/openal32.dll")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITQUEST_OPENAL_DLL "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x64/openal32.dll")
  else()
    set(BITQUEST_OPENAL_DLL "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x86/openal32.dll")
  endif()
  if(EXISTS "${BITQUEST_OPENAL_DLL}")
    add_custom_command(TARGET BITQUEST POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${BITQUEST_OPENAL_DLL}" "$<TARGET_FILE_DIR:BITQUEST>"
    )
  endif()
endif()

# One-command packager: cmake --build build --target package_game
# macOS universal (Intel + Apple Silicon): configure with -DBITQUEST_UNIVERSAL=ON
# Windows: run the same target on Windows, or use .github/workflows/package.yml
if(APPLE)
  set(BITQUEST_BUNDLE_DIR_ARG "$<TARGET_BUNDLE_DIR:BITQUEST>")
else()
  set(BITQUEST_BUNDLE_DIR_ARG "")
endif()
if(NOT DEFINED BITQUEST_OPENAL_DLL)
  set(BITQUEST_OPENAL_DLL "")
endif()
string(REPLACE ";" "+" BITQUEST_ARCHS_ARG "${CMAKE_OSX_ARCHITECTURES}")

add_custom_target(package_game
  DEPENDS BITQUEST
  COMMAND ${CMAKE_COMMAND}
    "-DBITQUEST_EXE=$<TARGET_FILE:BITQUEST>"
    "-DBITQUEST_FILE_DIR=$<TARGET_FILE_DIR:BITQUEST>"
    "-DBITQUEST_BUNDLE_DIR=${BITQUEST_BUNDLE_DIR_ARG}"
    "-DRES_DIR=$<TARGET_FILE_DIR:BITQUEST>/res"
    "-DSFML_LIB_DIR=${CMAKE_BINARY_DIR}/lib/sfml/lib"
    "-DSFML_SOURCE_DIR=${PROJECT_SOURCE_DIR}/lib/sfml"
    "-DBITQUEST_RUNTIME_FILES=$<TARGET_FILE:sfml-system>|$<TARGET_FILE:sfml-window>|$<TARGET_FILE:sfml-graphics>|$<TARGET_FILE:sfml-audio>"
    "-DOPENAL_DLL=${BITQUEST_OPENAL_DLL}"
    "-DBITQUEST_ARCHS=${BITQUEST_ARCHS_ARG}"
    "-DOUTPUT_DIR=${CMAKE_BINARY_DIR}/dist"
    -P "${PROJECT_SOURCE_DIR}/cmake/package_game.cmake"
  COMMENT "Packaging BitQuest into ${CMAKE_BINARY_DIR}/dist"
  VERBATIM
)
