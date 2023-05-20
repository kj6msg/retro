################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

cmake_minimum_required(VERSION 3.23)
project(retro LANGUAGES CXX VERSION 0.1.0 DESCRIPTION "Retro Computing Library")

include(GNUInstallDirs)
find_package(SDL2 REQUIRED COMPONENTS SDL2 CONFIG)

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
