file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/tests/../src/main.cpp)

find_package(glm CONFIG REQUIRED)

add_executable(CanvasTests canvasTests.cpp ${SOURCES})
add_executable(TupleTests tupleTests.cpp)

TARGET_LINK_LIBRARIES(CANVASTESTS PRIVATE GLM)
TARGET_LINK_LIBRARIES(TUPLETESTS PRIVATE GLM)

add_test(canvasTests TupleTests)
add_test(tupleTests TupleTests)