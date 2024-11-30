

function(get_all_targets var)
    set(targets)
    get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        get_all_targets_recursive(${targets} ${subdir})
    endforeach()

    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()

cmake_minimum_required (VERSION 3.20)
project (Training LANGUAGES CXX)

#include(CTest)
enable_testing()

set(VCPKG_TARGET_TRIPLET x64-windows-static)
message( STATUS  "Path to vcpkg root" $ENV{VCPKG_ROOT})
include( $ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake)
include_directories( ${INCLUDE_DIRECTORIES}  $ENV{VCPKG_ROOT}\\installed\\x64-windows-static\\include\\ )
find_package(Catch2 CONFIG REQUIRED)
include(Catch)
add_subdirectory (Source/Algorithms)
add_subdirectory (Source/CppFeatures)
add_subdirectory (Source/ParaPython)
 

get_all_targets(all_targets)

foreach(exec  ${all_targets})
    message( STATUS ${exec})
       # Установка 20 стандарта С++
       set_target_properties (${exec} PROPERTIES CXX_STANDARD 20 CMAKE_CXX_STANDARD_REQUIRED ON)
       # Установка статической линковки для MSVC
       set_property (TARGET ${exec} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
       # Установка флага многопоточной компиляции
       if (MSVC)
       target_compile_options (${exec} PRIVATE "/MP")
       endif()
       if(MSVC)
   target_compile_options(${exec} PRIVATE /W4 )
       else()
    target_compile_options(${exec} PRIVATE -Wall -Wextra -Wpedantic -Werror)
       endif()
       target_link_libraries(${exec}  PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

endforeach()