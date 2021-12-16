cmake_minimum_required(VERSION 3.11)

include(cmake/macros.cmake)
include(cmake/platform/windows.cmake)

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(GTEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/googletest/")

set_install_prefix(${ROOT})

set(SOLUTION_NAME "ycl_test")

project(${SOLUTION_NAME})

set(BUILD_GMOCK OFF CACHE BOOL "do not use gmock")
set(INSTALL_GTEST OFF CACHE BOOL "do not install gtest")
add_subdirectory(googletest)

add_subdirectory(test)
