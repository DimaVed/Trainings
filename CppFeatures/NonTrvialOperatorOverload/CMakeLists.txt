cmake_minimum_required(VERSION 3.21)

project(
	OperatorOverload
    LANGUAGES CXX
)




file (GLOB_RECURSE  Src  *.cpp  *.h )
set (exec OperatorOverload)
add_executable(${exec}  ${Src} )

