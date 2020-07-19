cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-openmp-simd")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s BUILD_AS_WORKER=1")

set(TO_NATIVE ON)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
	include/*.h
	include/*.cpp
)

# message("${SOURCES}")
# add_subdirectory(libnoise)
# find_package(LibNoise)

# add_subdirectory(cereal)
# add_subdirectory(cpp-taskflow)

add_executable(RayTracer ${SOURCES})


if(${TO_NATIVE})
	message(STATUS "Setting compilation target to native binary")
	# set_target_properties(RayTracer PROPERTIES LINK_FLAGS -lc++fs)
	target_link_libraries(RayTracer PRIVATE "pthread")

	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	if (WIN32)
		#find_package(TBB CONFIG REQUIRED)
	endif (WIN32)
	# find_package(ParallelSTL CONFIG REQUIRED)
	# find_package(glfw3 CONFIG REQUIRED)
	find_package(SDL2 CONFIG REQUIRED)
	# find_package(SDL2_image REQUIRED)
	find_package(yaml-cpp CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)

	find_package(TBB CONFIG REQUIRED)

	# find_package(CUDA)
	# find_package(assimp CONFIG REQUIRED)
	#find_package(Vulkan REQUIRED)
	#add_subdirectory(parallelstl)
	#add_subdirectory(glfw)


	#target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/glfw/include C:\\VulkanSDK\\1.1.130.0\\Include)
	#target_compile_definitions(RayTracer PRIVATE VK_USE_PLATFORM_WIN32_KHR)
	#target_include_directories(RayTracer PRIVATE Vulkan::Vulkan)
	target_include_directories(RayTracer PRIVATE ${SDL2_INCLUDE_DIRS} )
	#target_link_libraries(RayTracer PRIVATE $<$<CXX_COMPILER_ID:Clang>:rt>)
	if (WIN32)
		target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb SDL2::SDL2main SDL2::SDL2-static)
	else()
		target_link_libraries(RayTracer PRIVATE ${GLM_LIBRARIES} tbb yaml-cpp ${SDL2_LIBRARIES} )
	endif (WIN32)
	
	
else()
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")

	add_subdirectory(yaml-cpp)
	add_subdirectory(glm)

	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "--bind -s WASM=1 -s BUILD_AS_WORKER=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "--bind -s WASM=1 -s BUILD_AS_WORKER=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	target_link_libraries(RayTracer PRIVATE glm yaml-cpp)
	
endif()


target_include_directories(RayTracer PRIVATE include)
# target_include_directories(RayTracer PRIVATE cpp-taskflow include)
# target_link_libraries(RayTracer PRIVATE glm yaml-cpp )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
