# Установка минимальной версии CMake 
cmake_minimum_required (VERSION 3.20)
project (Proj LANGUAGES CXX)

# создание решения
set (SOLUTION solution)
add_executable (${SOLUTION} 
 src/main.cpp
 
)
# Установка 20 стандарта С++
set_target_properties (${SOLUTION} PROPERTIES CXX_STANDARD 20 CMAKE_CXX_STANDARD_REQUIRED ON)
# Установка статичской линковки для MSVC
set_property (TARGET ${SOLUTION} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# Установка флага многопоточной компилции
if (MSVC)
 target_compile_options (${SOLUTION} PRIVATE "/MP")
endif()
# настройка статической линковки Boost
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    ON)
# Поиск компонентов boost
find_package(Boost 1.79.0 COMPONENTS program_options locale)
# Линковка boost 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
    target_link_libraries(${SOLUTION} ${Boost_LIBRARIES})
else()
    message(STATUS "Fail  Boost weren't find!")
endif()

