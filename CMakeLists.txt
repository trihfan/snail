cmake_minimum_required(VERSION 3.12)

set(version "1.0.0")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
project(snail VERSION ${version} LANGUAGES CXX)
enable_testing()

# windows specific
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MT /NODEFAULTLIB:library")
endif()

# use C++17
set (CMAKE_CXX_STANDARD 17)

# options
option(snail_build_viewer "build the included viewer, required serialize option to be ON" ON)
option(snail_build_test "build the snail tests" ON)
option(snail_json "enable json serialize with nlohmann::json" ON)
option(snail_chai "enable scripting with chaiscript" ON)

if (${snail_build_viewer})
    if (NOT ${snail_json} OR NOT ${snail_chai})
        message(FATAL_ERROR "to buid the viewer, the json and chai options are required")
    endif()
endif()

# packages
find_package(Eigen3 CONFIG REQUIRED)

if(${snail_chai})
    #find_package(ChaiScript CONFIG REQUIRED)
    add_definitions(-DSNAIL_CHAI)
endif()

if(${snail_json})
    find_package(nlohmann_json CONFIG REQUIRED)
    add_definitions(-DSNAIL_JSON)
endif()

# verbosity level
set(snail_verbosity_level 0 CACHE STRING "Snail verbose level: 0=no, 1=error only, 2=error + info, 3=debug output scripts, 3=all debug (slow!)")
set_property(CACHE snail_verbosity_level PROPERTY STRINGS 0 1 2 3 4)
add_definitions(-DVERBOSITY_LEVEL=${snail_verbosity_level})

if(${snail_json} AND ${snail_chai} AND ${snail_verbosity_level} GREATER 2)
    set(snail_debug_output_directory "${PROJECT_BINARY_DIR}/debug/" CACHE STRING "path to write debug files")
    add_definitions(-DDEBUG_OUTPUT_DIRECTORY="${snail_debug_output_directory}")
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

if(${snail_build_viewer})
    add_subdirectory(viewer)
endif()

if(${snail_build_tests})
    add_subdirectory(tests)
    add_subdirectory(googletest)
endif()
