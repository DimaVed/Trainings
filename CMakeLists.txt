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


# Установка минимальной версии CMake 


cmake_minimum_required (VERSION 3.20)
project (Training LANGUAGES CXX)


add_subdirectory (Algorithms)
add_subdirectory (CppFeatures)
add_subdirectory (ParaPython)


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
	
endforeach()