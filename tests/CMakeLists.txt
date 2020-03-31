project(tests)

# include
include_directories(${tests_path})

# make executable
file(GLOB_RECURSE sources *.h  *.cpp)
add_executable(tests ${sources})

# add libraries
target_link_libraries(tests gtest)
target_link_libraries(tests gtest_main)
target_link_libraries(tests snail)

# set output directories
set_target_properties(tests PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(tests PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

# set test
add_test(test tests)
