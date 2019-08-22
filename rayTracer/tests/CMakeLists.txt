file(GLOB SOURCES
    ../src/*.h
    ../src/*.cpp
)

list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/tests/../src/main.cpp)

add_executable(CanvasTests canvasTests.cpp ${SOURCES})
add_executable(TupleTests tupleTests.cpp)

add_test(canvasTests TupleTests)
add_test(tupleTests TupleTests)