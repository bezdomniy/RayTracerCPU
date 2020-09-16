cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
endif()

set(USE_THREADS ON)


#set(CMAKE_CXX_FLAGS "-openmp-simd")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -msimd128")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")
# CMAKE_EXE_LINKER_FLAGS_RELEASE

add_subdirectory("src/raytracer")
add_subdirectory("src/runner")

# if(${TO_NATIVE})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	message(STATUS "Setting compilation target to WASM")
	# set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")

	add_subdirectory(yaml-cpp)
	add_subdirectory(glm)
	if (${USE_THREADS})
		add_subdirectory(cpp-taskflow)
	endif()
else()
	message(STATUS "Setting compilation target to native binary")

	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	# set(TBB_DIR /home/ilia/dev/tools/tbb/cmake)
	#set(TBB_DIR C:\\dev\\tools\\tbb\\cmake)
	#set(TBB_DIR /home/ilia/vcpkg/installed/x64-linux/share/tbb)

	if (WIN32)
		#find_package(TBB CONFIG REQUIRED)
	endif (WIN32)
	find_package(SDL2 CONFIG REQUIRED)
	find_package(yaml-cpp CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)
	find_package(TBB CONFIG REQUIRED)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
