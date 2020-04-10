project(viewer)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# add linux libs
if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lstdc++ -Wl,--no-as-needed -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl")
endif()

# add apple libs
if(APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()

# include
include_directories(${viewer_path})
# link
link_directories(viewer ${lib_output_directory})

# make executable
file(GLOB_RECURSE sources  *.h  *.cpp *.c *.glsl)
add_executable(viewer ${sources})

# add libraries
target_link_libraries(viewer glfw)
target_link_libraries(viewer OpenGL::GL)
target_link_libraries(viewer snail)

# set output directories
set_target_properties(viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

# add shaders
install(DIRECTORY "${viewer_path}/shaders" DESTINATION "${bin_output_directory}")
