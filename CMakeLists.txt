cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(rb_set LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

# Test programs not compiled by default for faster compilation.
option(BUILD_TEST_PROGRAM "Build container test programs" false)

# Setting up test and coverage.
find_package(Catch2 REQUIRED)
option(ENABLE_COVERAGE "Enable coverage" false)
option(ENABLE_DEBUG "Enable sanitizers" false)

# Turn on sanitizers when flag set.
if (ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE debug)

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set (CMAKE_CXX_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set (CMAKE_CXX_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} 
    -fsanitize=undefined")
endif()

# Setting parameters in test build.
if (ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE debug)

  include(CodeCoverage.cmake)
  APPEND_COVERAGE_COMPILER_FLAGS()
endif()

# Setting include directories.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Executables for this project.
if (BUILD_TEST_PROGRAM)
  add_executable(test_rb_tree test/test_rb_tree.cpp)
  add_executable(test_rb_set test/test_rb_set.cpp)
endif()
add_executable(script_intpr app/script_intpr.cpp)

# Install program, demo script, and demo file into specified folder.
install(TARGETS script_intpr DESTINATION bin)
install(PROGRAMS demo DESTINATION bin)
install(PROGRAMS script/demo_script.txt DESTINATION bin/script)

# Enabling Lcov in test build.
if (ENABLE_COVERAGE)
  configure_file(script/demo_script.txt ${CMAKE_BINARY_DIR}/demo_script.txt
    COPYONLY)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/run_tests
    EXECUTABLE_ARGS ${CMAKE_CURRENT_BINARY_DIR}
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage

    # Append executables to target coverage.
    DEPENDENCIES test_rb_tree test_rb_set script_intpr)
endif()