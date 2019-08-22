find_package(glm CONFIG REQUIRED)

add_executable(RayTracer main.cpp)
target_link_libraries(RayTracer PRIVATE glm)
