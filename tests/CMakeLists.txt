cmake_minimum_required(VERSION 3.5)

project(sumdR2_test VERSION 0.0.1 LANGUAGES C CXX)

# This project can use C11, but will gracefully decay down to C89.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)

include(CheckCXXSymbolExists)
# Using check_cxx_symbol_exists() instead of check_c_symbol_exists() because
# we're including the header from C++, and feature detection should use the same
# compiler language that the project will use later. Principles aside, some
# versions of do not expose fdatasync() in <unistd.h> in standard C mode
# (-std=c11), but do expose the function in standard C++ mode (-std=c++11).
check_cxx_symbol_exists(fdatasync "unistd.h" HAVE_FDATASYNC)
check_cxx_symbol_exists(F_FULLFSYNC "fcntl.h" HAVE_FULLFSYNC)
check_cxx_symbol_exists(O_CLOEXEC "fcntl.h" HAVE_O_CLOEXEC)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

set(CMAKE_C_FLAGS "-O0 -g")
set(CMAKE_CXX_FLAGS "-O0 -g")

include_directories(
  "/home/zhijieli/.conda/envs/zhijieli_stxflow3.0/include/python2.7/"
  "${PROJECT_SOURCE_DIR}/googletest-release-1.10.0/googletest/include/"
  "${PROJECT_SOURCE_DIR}/cvxopt_core/"
)

link_directories(
  "/home/zhijieli/.conda/envs/zhijieli_stxflow3.0/lib/"
  "${PROJECT_SOURCE_DIR}/googletest-release-1.10.0/build/lib/"
)

add_executable(sumdR2_test "")
target_sources(sumdR2_test
  PRIVATE
    "${PROJECT_SOURCE_DIR}/test_sumdR2.cpp"
    "${PROJECT_SOURCE_DIR}/../c_extension/sumdR2.cpp"
    "${PROJECT_SOURCE_DIR}/cvxopt_core/base.c"
    "${PROJECT_SOURCE_DIR}/cvxopt_core/dense.c"
    "${PROJECT_SOURCE_DIR}/cvxopt_core/sparse.c"
)

target_link_libraries(sumdR2_test gtest)
target_link_libraries(sumdR2_test pthread)
target_link_libraries(sumdR2_test python2.7)
target_link_libraries(sumdR2_test mkl_rt)

