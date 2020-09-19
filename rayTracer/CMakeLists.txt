cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
endif()

set(USE_THREADS OFF)

if (${USE_THREADS})
	set(CMAKE_CXX_FLAGS_DEBUG "-g -s USE_PTHREADS=1")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto -s USE_PTHREADS=1")
else()
	#set(CMAKE_CXX_FLAGS "-openmp-simd")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -march=native")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto -march=native -Wpadded") # -Wpadded
endif()

set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")



# if(${TO_NATIVE})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	message(STATUS "Setting compilation target to WASM")
	add_subdirectory("src/runner")
	add_subdirectory("src/raytracer")
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

	# add_subdirectory(glm)
	find_package(glm CONFIG REQUIRED)
	find_package(TBB CONFIG REQUIRED)

	add_subdirectory("src/raytracer")
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
