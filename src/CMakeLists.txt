################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

add_library(retro)
add_library(retro::retro ALIAS retro)

include(GNUInstallDirs)

set_target_properties(retro PROPERTIES
    CXX_EXTENSIONS OFF
    INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_LIBDIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_LIBDIR}"
)

set(MODULE_OUTPUT_DIRECTORY
    "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_INSTALL_LIBDIR}/cmake/retro"
)
set(INSTALL_MODULEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/retro")

target_sources(retro PRIVATE
    color.cpp
    font.cpp
    glyphs.cpp
    sdl2.cpp
    sprite.cpp
    vga.cpp
)

target_sources(retro PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
    FILES
    "${PROJECT_SOURCE_DIR}/include/retro/color.hpp"
    "${PROJECT_SOURCE_DIR}/include/retro/font.hpp"
    "${PROJECT_SOURCE_DIR}/include/retro/retro.hpp"
    "${PROJECT_SOURCE_DIR}/include/retro/sdl2.hpp"
    "${PROJECT_SOURCE_DIR}/include/retro/sprite.hpp"
    "${PROJECT_SOURCE_DIR}/include/retro/vga.hpp"
)

target_compile_features(retro PUBLIC cxx_std_23)
target_compile_options(retro PRIVATE
    "-Wall"
    "-Wextra"
    "-Wconversion"
    "-Wold-style-cast"
)
target_link_libraries(retro PUBLIC SDL2::SDL2)


################################################################################
install(TARGETS retro EXPORT retroTargets FILE_SET HEADERS)
install(EXPORT retroTargets
    FILE retroTargets.cmake
    NAMESPACE retro::
    DESTINATION "${INSTALL_MODULEDIR}"
)

include(CMakePackageConfigHelpers)

configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/retroConfig.cmake.in"
    "${MODULE_OUTPUT_DIRECTORY}/retroConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_MODULEDIR}"
    NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
    "${MODULE_OUTPUT_DIRECTORY}/retroConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${MODULE_OUTPUT_DIRECTORY}/retroConfig.cmake"
    "${MODULE_OUTPUT_DIRECTORY}/retroConfigVersion.cmake"
    DESTINATION "${INSTALL_MODULEDIR}"
)

export(EXPORT retroTargets FILE "${MODULE_OUTPUT_DIRECTORY}/retroTargets.cmake")
