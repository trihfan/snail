project(snail)

# use C++17
set (CMAKE_CXX_STANDARD 17)

# make lib
add_library(snail INTERFACE)

# target
target_include_directories(snail INTERFACE ../)
target_include_directories(snail INTERFACE ${EIGEN3_INCLUDE_DIR})

if(${snail_chai})
    target_include_directories(snail INTERFACE ${CHAISCRIPT_INCLUDE_DIR})
endif()

if(${snail_json})
    target_link_libraries(snail INTERFACE nlohmann_json::nlohmann_json)
endif()

target_compile_features(snail INTERFACE cxx_std_17)
