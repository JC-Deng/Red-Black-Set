cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(rb_set LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

# # Setting up test and coverage.
# find_package(Catch2 REQUIRED)
# option(ENABLE_COVERAGE "Enable coverage" false)

# # Setting parameters in test build.
# if (ENABLE_COVERAGE)
#   set(CMAKE_BUILD_TYPE debug)

#   include(CodeCoverage.cmake)
#   APPEND_COVERAGE_COMPILER_FLAGS()

#   # Turn on sanitizers for test build.
#   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#   set (CMAKE_CXX_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

#   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
#   set (CMAKE_CXX_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} 
#     -fsanitize=undefined")
# endif()

# Setting include directories.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Executables for this project.
add_executable(test_rb_tree test/test_rb_tree.cpp)
add_executable(test_rb_set test/test_rb_set.cpp)
add_executable(script_intpr app/script_intpr.cpp)
# add_executable(test_matrix_multiply app/test_matrix_multiply.cpp)
# add_executable(test_fft app/test_fft.cpp)

# # Enabling Lcov in test build.
# if (ENABLE_COVERAGE)
#   setup_target_for_coverage_lcov(
#     NAME coverage
#     EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/run_tests
#     EXECUTABLE_ARGS ${CMAKE_CURRENT_BINARY_DIR}
#     LCOV_ARGS -rc lcov_branch_coverage=1
#     GENHTML_ARGS --branch-coverage

#     # Append executables to target coverage.
#     DEPENDENCIES test_matrix_transpose test_matrix_multiply test_fft)
# endif()