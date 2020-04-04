cmake_minimum_required(VERSION 3.12)

set(version "1.0.0")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
project(snail VERSION ${version} LANGUAGES CXX)
enable_testing()

# use C++17
set (CMAKE_CXX_STANDARD 17)

# packages
find_package(Eigen3 REQUIRED)

# options
option(snail_verbose "" true)

if (${snail_verbose})
    add_definitions(-DVERBOSITY_LEVEL_0)
endif()

# custom variables
set(top_binary_dir "${PROJECT_BINARY_DIR}")
set(snail_path "${PROJECT_SOURCE_DIR}/snail")
set(viewer_path "${PROJECT_SOURCE_DIR}/viewer")
set(tests_path "${PROJECT_SOURCE_DIR}/tests")

# set the lib and bin directories
set(lib_output_directory "${top_binary_dir}/lib")
set(bin_output_directory "${top_binary_dir}/bin")

# add the lib
add_subdirectory(snail)
add_subdirectory(viewer)
add_subdirectory(tests)
add_subdirectory(googletest)
