project(snail)

# use C++17
set (CMAKE_CXX_STANDARD 17)

# make lib
add_library(snail INTERFACE)

# target
target_include_directories(snail INTERFACE ../)
target_include_directories(snail INTERFACE ${GLM_INCLUDE_DIRS})
target_include_directories(snail INTERFACE ${EIGEN3_INCLUDE_DIR})
target_compile_features(snail INTERFACE cxx_std_17)
