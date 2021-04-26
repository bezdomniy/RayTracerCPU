cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
endif()

set(USE_THREADS ON)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	set(USE_THREADS ON)
endif()

if (${USE_THREADS} AND ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -pthread")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto -pthread -s PROXY_TO_PTHREAD")
else()
	#set(CMAKE_CXX_FLAGS "-openmp-simd")
	# set(CMAKE_CXX_FLAGS_DEBUG "-g")

	if (WIN32)
		set(CMAKE_CXX_FLAGS_RELEASE "/O2 /fp:fast /GL /arch:AVX2")
	else()
		set(CMAKE_CXX_FLAGS_DEBUG "-g")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto") # -Wpadded
	endif()
endif()

set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")


add_subdirectory(yaml-cpp)
add_subdirectory(glm)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	message(STATUS "Setting compilation target to WASM")
	
	# set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")

	if (NOT ${USE_THREADS}) 
		add_subdirectory("src/runner")
	endif()
else()
	message(STATUS "Setting compilation target to native binary")

	find_package(SDL2 CONFIG REQUIRED)
	add_subdirectory(yaml-cpp)
	add_subdirectory(glm)

	set(mathfu_build_benchmarks OFF CACHE BOOL "")
	set(mathfu_build_tests oFF CACHE BOOL "")
	add_subdirectory(./mathfu)
	add_subdirectory("src/raytracer")
endif()

add_subdirectory("src/raytracer")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
