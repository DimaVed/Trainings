cmake_minimum_required(VERSION 3.21)

project(
	Vectors
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )
add_executable(Vectors  ${Src} )

