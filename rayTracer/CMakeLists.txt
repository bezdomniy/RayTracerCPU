cmake_minimum_required(VERSION 3.12)
project(RayTracer VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(glm CONFIG REQUIRED)
# add_executable(RayTracer main.cpp)
# add_subdirectory(src) 
# add_subdirectory(tests)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

# message(${SOURCES})

add_executable(RayTracer ${SOURCES})
target_link_libraries(RayTracer PRIVATE glm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
