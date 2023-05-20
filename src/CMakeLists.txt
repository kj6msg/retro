################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

add_library(retro SHARED)

set_target_properties(retro PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
set_target_properties(retro PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
)

target_sources(retro PRIVATE
    sdl2.cpp
    vga.cpp
)

target_sources(retro PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
    ${PROJECT_SOURCE_DIR}/include/retro/retro.hpp
    ${PROJECT_SOURCE_DIR}/include/retro/sdl2.hpp
    ${PROJECT_SOURCE_DIR}/include/retro/vga.hpp
)

target_compile_features(retro PRIVATE cxx_std_20)
target_compile_options(retro PRIVATE -Wall -Wextra)
target_link_libraries(retro PUBLIC SDL2::SDL2)

install(TARGETS retro FILE_SET HEADERS)
