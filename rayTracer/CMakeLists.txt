cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(TO_NATIVE ON)

# include(CTest)
# enable_testing()
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# message("${CMAKE_SOURCE_DIR}")
# message("${CMAKE_SOURCE_DIR}")

file(GLOB SOURCES
    src/*.h
    src/*.cpp
	include/*.h
	include/*.cpp
)

# message("${SOURCES}")
# add_subdirectory(libnoise)
# find_package(LibNoise)
add_subdirectory(yaml-cpp)
add_subdirectory(glm)
add_subdirectory(cpp-taskflow)

add_executable(RayTracer ${SOURCES})

if(${TO_NATIVE})
	message(STATUS "Setting compilation target to native binary")
<<<<<<< HEAD
	# set_target_properties(RayTracer PROPERTIES LINK_FLAGS -lc++fs)
	target_link_libraries(RayTracer PRIVATE "pthread")
=======

	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	# set(TBB_DIR /home/ilia/dev/tools/tbb/cmake)
	#set(TBB_DIR C:\\dev\\tools\\tbb\\cmake)
	#set(TBB_DIR /home/ilia/vcpkg/installed/x64-linux/share/tbb)

	if (WIN32)
		find_package(TBB CONFIG REQUIRED)
	endif (WIN32)
	find_package(ParallelSTL CONFIG REQUIRED)
	# find_package(glfw3 CONFIG REQUIRED)
	find_package(SDL2 CONFIG REQUIRED)
	find_package(yaml-cpp CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)
	find_package(assimp CONFIG REQUIRED)
	#find_package(Vulkan REQUIRED)
	#add_subdirectory(parallelstl)
	#add_subdirectory(glfw)

	#target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/glfw/include C:\\VulkanSDK\\1.1.130.0\\Include)
	#target_compile_definitions(RayTracer PRIVATE VK_USE_PLATFORM_WIN32_KHR)
	#target_include_directories(RayTracer PRIVATE Vulkan::Vulkan)
	target_include_directories(RayTracer PRIVATE ${SDL2_INCLUDE_DIRS})
	#target_link_libraries(RayTracer PRIVATE $<$<CXX_COMPILER_ID:Clang>:rt>)
	target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb pstl::ParallelSTL SDL2::SDL2main SDL2::SDL2-static assimp::assimp)
>>>>>>> 336f3a3... preparing to implement obj
else()
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "--bind -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s EXTRA_EXPORTED_RUNTIME_METHODS=FS -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "--bind -s WASM=1 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s EXTRA_EXPORTED_RUNTIME_METHODS=FS -s ALLOW_MEMORY_GROWTH=1")
endif()

# target_include_directories(RayTracer PRIVATE ${LIBNOISE_INCLUDE_DIR})
# target_link_libraries(RayTracer PRIVATE glm yaml-cpp ${LIBNOISE_LIBRARY})

target_include_directories(RayTracer PRIVATE cpp-taskflow)
target_link_libraries(RayTracer PRIVATE glm yaml-cpp )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
