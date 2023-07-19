################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

################################################################################
add_executable(xor_demo xor_demo.cpp xor_demo.hpp)
set_target_properties(xor_demo PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
)
target_compile_features(xor_demo PUBLIC cxx_std_20)
target_compile_options(xor_demo PRIVATE
    "-Wall"
    "-Wextra"
    "-Wconversion"
    "-Wold-style-cast"
)
target_link_libraries(xor_demo PRIVATE retro::retro)


################################################################################
add_executable(plasma plasma.cpp plasma.hpp)
set_target_properties(plasma PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
)
target_compile_features(plasma PUBLIC cxx_std_20)
target_compile_options(plasma PRIVATE
    "-Wall"
    "-Wextra"
    "-Wconversion"
    "-Wold-style-cast"
)
target_link_libraries(plasma PRIVATE retro::retro)


################################################################################
add_executable(fire fire.cpp fire.hpp)
set_target_properties(fire PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
)
target_compile_features(fire PUBLIC cxx_std_20)
target_compile_options(fire PRIVATE
    "-Wall"
    "-Wextra"
    "-Wconversion"
    "-Wold-style-cast"
)
target_link_libraries(fire PRIVATE retro::retro)


################################################################################
add_executable(font_demo font_demo.cpp font_demo.hpp)
set_target_properties(font_demo PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
)
target_compile_features(font_demo PUBLIC cxx_std_20)
target_compile_options(font_demo PRIVATE
    "-Wall"
    "-Wextra"
    "-Wconversion"
    "-Wold-style-cast"
)
target_link_libraries(font_demo PRIVATE retro::retro)
