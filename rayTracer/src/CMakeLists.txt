find_package(glm CONFIG REQUIRED)

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(RayTracer ${SOURCES})
target_link_libraries(RayTracer PRIVATE glm)
