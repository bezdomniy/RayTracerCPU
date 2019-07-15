# CMakeList.txt : CMake project for Game, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (Game)

# Add source to this project's executable.
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_compile_options("-Wpragma-pack")

add_executable(Game ${SOURCES})
#add_executable (Game "./src/main.cpp")

# TODO: Add tests and install targets if needed.
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Game PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(Game PRIVATE SDL2::SDL2_image)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Game PRIVATE nlohmann_json nlohmann_json::nlohmann_json)