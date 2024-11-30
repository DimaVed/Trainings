# Установка минимальной версии CMake 
cmake_minimum_required (VERSION 3.20)
project (CppFeatures LANGUAGES CXX)

add_subdirectory (OperatorOverload)
add_subdirectory (Range)
add_subdirectory (Split)
add_subdirectory (Vectors)
add_subdirectory (Preprocessor)