# Top-level CMakeLists file for the Chapter 2 example content.
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.21)

project(
    Algorithms
    LANGUAGES CXX
)

# Option to exclude driver application from build.



add_subdirectory(AStar)
add_subdirectory(Contraction)
add_subdirectory(DisJointSet)
add_subdirectory(RBTREE)
# Include driver_app subdirectory if CHAPTER2_BUILD_DRIVER_APPLICATION flag is
# enabled.
if(CHAPTER2_BUILD_DRIVER_APPLICATION)
    add_subdirectory(driver_app)
endif()