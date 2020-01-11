cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(TO_NATIVE OFF)

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

add_executable(RayTracer ${SOURCES})

if(${TO_NATIVE})
	message(STATUS "Setting compilation target to native binary")
	# set_target_properties(RayTracer PROPERTIES LINK_FLAGS -lc++fs)
	# target_link_libraries(RayTracer PRIVATE "stdc++fs")
else()
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "--bind -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s EXTRA_EXPORTED_RUNTIME_METHODS=FS -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "--bind -s WASM=1 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s EXTRA_EXPORTED_RUNTIME_METHODS=FS -s ALLOW_MEMORY_GROWTH=1")
endif()

# target_include_directories(RayTracer PRIVATE ${LIBNOISE_INCLUDE_DIR})
# target_link_libraries(RayTracer PRIVATE glm yaml-cpp ${LIBNOISE_LIBRARY})

target_include_directories(RayTracer PRIVATE )
target_link_libraries(RayTracer PRIVATE glm yaml-cpp )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
