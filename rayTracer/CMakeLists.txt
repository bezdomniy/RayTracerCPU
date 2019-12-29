cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

option(TO_NATIVE "Compiles to native (Not WASM)" ON)

include(CTest)
enable_testing()

# To find and use libnoise
# find_path(LIBNOISE_INCLUDE_DIR NAMES "noise.h" PATHS C:/Users/ilias/source/repos/vcpkg/installed/x86-windows-static/include/noise)
# find_library(LIBNOISE_LIBRARY NAMES noise-static PATHS C:/Users/ilias/source/repos/vcpkg/installed/x86-windows-static/lib)
# find_path(LIBNOISE_INCLUDE_DIR NAMES "noise.h" PATHS /usr/include/libnoise)
# find_library(LIBNOISE_LIBRARY NAMES noise PATHS /usr/lib/libnoise)


find_package(glm CONFIG REQUIRED)
# find_package(yaml-cpp REQUIRED)

add_subdirectory(yaml-cpp)
# add_subdirectory(glm)
# find_package(yaml-cpp REQUIRED)


# add_executable(RayTracer main.cpp)
# add_subdirectory(src) 
# add_subdirectory(tests)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/archive/*.h
    src/archive/*.cpp
)

# message(${LIBNOISE_INCLUDE_DIR})
# message(${LIBNOISE_LIBRARY})
# add_subdirectory(libnoise)

add_executable(RayTracer ${SOURCES})

if(TO_NATIVE)
	message(STATUS "Setting compilation target to native binary")
	# set(CMAKE_EXECUTABLE_SUFFIX ".js")
	# set_target_properties(RayTracer PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[_main]'")
else(TO_NATIVE)
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS "-s WASM=1 -s BINARYEN_METHOD='native-wasm' -s EXPORTED_FUNCTIONS='[_main]' -s EXTRA_EXPORTED_RUNTIME_METHODS=FS")
endif(TO_NATIVE)

# target_include_directories(RayTracer PRIVATE ${LIBNOISE_INCLUDE_DIR})
# target_link_libraries(RayTracer PRIVATE glm yaml-cpp ${LIBNOISE_LIBRARY})

target_include_directories(RayTracer PRIVATE )
target_link_libraries(RayTracer PRIVATE glm yaml-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
