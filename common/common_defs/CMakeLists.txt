cmake_minimum_required(VERSION 3.13)

add_library(common_defs INTERFACE)

# Find directories for '#include', this directroy for the simple lib
target_include_directories(common_defs INTERFACE .)