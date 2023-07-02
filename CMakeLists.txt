# Установка минимальной версии CMake 
cmake_minimum_required (VERSION 3.20)
project (Proj LANGUAGES CXX)

# создание решения
set (OperatorOverload OperatorOverload)
add_executable (${OperatorOverload} 
 CppFeatures/NonTrvialOperatorOverload/main.cpp
)
set (DisJointSet DisJointSet)
add_executable (${DisJointSet} 
 Algos/DisJointSet/main.cpp
)

set (Contraction Contraction)
add_executable (${Contraction} 
 Algos/Contraction/main.cpp
 Algos/Contraction/Contraction.cpp
 Algos/Contraction/Contraction.h
 Algos/Contraction/profile.h
 Algos/Contraction/test_runner.h
)
set (AStar AStar)
add_executable (${AStar} 
 Algos/AStar/main.cpp
 Algos/AStar/A_star.h
 Algos/AStar/A_star.cpp
 Algos/AStar/profile.h
 Algos/AStar/test_runner.h
)

set(executables ${OperatorOverload} ${DisJointSet}
${Contraction} ${AStar} )

# Для всех исполняемых файлов
foreach(exec  ${executables})
    message( STATUS ${exec})
	# Установка 20 стандарта С++
	set_target_properties (${exec} PROPERTIES CXX_STANDARD 20 CMAKE_CXX_STANDARD_REQUIRED ON)
	# Установка статичской линковки для MSVC
	set_property (TARGET ${exec} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	# Установка флага многопоточной компилции
	if (MSVC)
	target_compile_options (${exec} PRIVATE "/MP")
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
		target_link_libraries(${exec} ${Boost_LIBRARIES})
	else()
		message(STATUS "Fail  Boost weren't find!")
	endif()
		
	endforeach()



