cmake_minimum_required(VERSION 3.21)

project(
	DisJointSet
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )
set (exec DisJointSet)
add_executable(${exec}  ${Src} )

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

 target_link_libraries(${exec} PUBLIC Catch2::Catch2 Catch2::Catch2WithMain)