cmake_minimum_required(VERSION 3.21)

project(
	Preprocessor
    LANGUAGES CXX
)




file (GLOB_RECURSE  Src  *.cpp  *.h )
set (exec Preprocessor)
add_executable(${exec}  ${Src} )
catch_discover_tests(${exec}  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
 target_compile_options(${exec} PRIVATE /W4 /WX )