cmake_minimum_required(VERSION 3.10)
project(RayTracer VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

# To find and use libnoise
# find_path(LIBNOISE_INCLUDE_DIR NAMES "noise.h" PATHS C:/Users/ilias/source/repos/vcpkg/installed/x86-windows-static/include/noise)
# find_library(LIBNOISE_LIBRARY NAMES noise-static PATHS C:/Users/ilias/source/repos/vcpkg/installed/x86-windows-static/lib)
find_path(LIBNOISE_INCLUDE_DIR NAMES "noise.h" PATHS /usr/include/libnoise)
find_library(LIBNOISE_LIBRARY NAMES noise PATHS /usr/lib/libnoise)


find_package(glm CONFIG REQUIRED)
# find_package(yaml-cpp REQUIRED)

add_subdirectory(yaml-cpp)
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

message(${LIBNOISE_INCLUDE_DIR})
message(${LIBNOISE_LIBRARY})
# add_subdirectory(libnoise)

add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE ${LIBNOISE_INCLUDE_DIR})
target_link_libraries(RayTracer PRIVATE glm yaml-cpp ${LIBNOISE_LIBRARY})


# link_libraries(RayTracer PRIVATE ${LIBNOISE_LIBRARY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
