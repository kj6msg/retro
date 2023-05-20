################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

add_executable(vga_test vga_test.cpp vga_test.hpp)

target_compile_features(vga_test PRIVATE cxx_std_20)
target_compile_options(vga_test PRIVATE -Wall -Wextra)
target_link_libraries(vga_test PRIVATE retro)
