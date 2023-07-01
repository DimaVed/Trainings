cmake_minimum_required (VERSION 3.20)

project (A_star LANGUAGES CXX)
add_executable (${PROJECT_NAME} 
 main.cpp
 profile.h
 test_runner.h
 A_star.h
 A_star.cpp
)
set_target_properties (${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CMAKE_CXX_STANDARD_REQUIRED ON)
