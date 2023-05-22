################################################################################
## Retro - Retro Computing Library
## Copyright (c) 2023 Ryan Clarke
################################################################################

add_executable(xor_demo xor_demo.cpp xor_demo.hpp)

target_compile_features(xor_demo PRIVATE cxx_std_20)
target_compile_options(xor_demo PRIVATE -Wall -Wextra)
target_link_libraries(xor_demo PRIVATE retro)

add_executable(plasma plasma.cpp plasma.hpp)

target_compile_features(plasma PRIVATE cxx_std_20)
target_compile_options(plasma PRIVATE -Wall -Wextra)
target_link_libraries(plasma PRIVATE retro)

add_executable(fire fire.cpp fire.hpp)

target_compile_features(fire PRIVATE cxx_std_20)
target_compile_options(fire PRIVATE -Wall -Wextra)
target_link_libraries(fire PRIVATE retro)
