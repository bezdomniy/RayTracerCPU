# CMakeList.txt : CMake project for Game, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)
# include("cmake/HunterGate.cmake")
# HunterGate(
#     URL "https://github.com/ruslo/hunter/archive/v0.16.15.tar.gz"
#     SHA1 "6974c2150fc0d3b09de3ad1efcbf15d360647ffa"
# )

project (Game)

# Add source to this project's executable.
file(GLOB SOURCES
    src/*.h
    src/*.cpp
    # dependencies/nlohmann/json.hpp
)

#add_compile_options("-Wpragma-pack")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")




find_package(SDL2 REQUIRED)

# include_directories(${SDL2_INCLUDE_DIR})
# target_link_libraries(Game PRIVATE ${SDL2_LIBRARIES})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
# target_link_libraries(Game PRIVATE ${SDL2_IMAGE_LIBRARIES})

find_package(GLEW REQUIRED)
# target_link_libraries(Game PRIVATE ${GLEW_LIBRARIES})

find_package(OpenGL REQUIRED)
# target_link_libraries(Game PRIVATE ${OPENGL_gl_LIBRARY})

find_package(glm CONFIG REQUIRED)
# target_link_libraries(Game PRIVATE glm)

find_package(assimp CONFIG REQUIRED)
# target_link_libraries(Game PRIVATE ${ASSIMP_LIBRARIES})

add_executable(Game ${SOURCES})

# message(SEND_ERROR ${GLEW_INCLUDE_DIR})

target_include_directories(Game PRIVATE dependencies/)
target_link_libraries(Game PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY} ${ASSIMP_LIBRARIES} glm)
#set(INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} )

#target_link_libraries(${PROJECT_NAME} ${LIBS})

# set(JSON_BuildTests OFF CACHE INTERNAL "")
# set(JSON_Install OFF CACHE INTERNAL "")
# target_link_libraries(Game PRIVATE nlohmann_json::nlohmann_json)

# hunter_add_package(nlohmann_json)
# find_package(nlohmann_json REQUIRED)
# add_library(Game)
# target_link_libraries(Game PRIVATE nlohmann_json::nlohmann_json)