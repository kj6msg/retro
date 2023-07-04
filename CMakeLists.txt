################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

cmake_minimum_required(VERSION 3.23...3.25 FATAL_ERROR)
project(retro VERSION 0.1.0 DESCRIPTION "Retro Computing Library" LANGUAGES CXX)

if(NOT (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    message(FATAL_ERROR "Only macOS is supported at this time.")
endif()

option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(BUILD_EXAMPLES "Build examples" OFF)


################################################################################
# Options for top project only
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Set default build type to "Debug" if part of a git clone
    if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
        set(DEFAULT_BUILD_TYPE "Debug")
    else()
        set(DEFAULT_BUILD_TYPE "Release")
    endif()

    # Check for specified build type and configurations
    if((NOT CMAKE_BUILD_TYPE) AND (NOT CMAKE_CONFIGURATION_TYPES))
        message(STATUS "No build type specified. Setting build type to '${DEFAULT_BUILD_TYPE}'.")
        set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE} CACHE STRING "Build type." FORCE)

        # Set the possible values of build type for cmake-gui/ccmake
        set_property(CACHE CMAKE_BUILD_TYPE
            PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
        )
    endif()

    # Be nice to IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # # Docs only available if this is the main app
    # find_package(Doxygen)
    # if(Doxygen_FOUND)
    #     add_subdirectory(docs)
    # else()
    #     message(STATUS "Doxygen not found, not building docs")
    # endif()
endif()


################################################################################
# Print build types
message(STATUS "Retro build configuration")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

if(BUILD_SHARED_LIBS)
    message(STATUS "Type: Shared")
else()
    message(STATUS "Type: Static")
endif()

message(STATUS "Build: ${CMAKE_BUILD_TYPE}")
list(POP_BACK CMAKE_MESSAGE_INDENT)

# Find dependencies
message(CHECK_START "Finding dependencies")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

message(CHECK_START "Finding SDL2")
find_package(SDL2 REQUIRED COMPONENTS SDL2 CONFIG)

message(CHECK_PASS "found: ${SDL2_DIR}")
list(POP_BACK CMAKE_MESSAGE_INDENT)


################################################################################
# Continue with rest of project
add_subdirectory("src")

if(BUILD_EXAMPLES)
    message(STATUS "Building examples")
    add_subdirectory("examples")
endif()
