file(GLOB RUNNERSOURCES
    *.h
    *.cpp
)

add_executable(Runner ${RUNNERSOURCES})

if (${USE_THREADS})
	add_compile_definitions(WITH_THREADS)
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 " )

    set_target_properties(Runner PROPERTIES LINK_FLAGS_RELEASE "-s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -flto -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
    set_target_properties(Runner PROPERTIES LINK_FLAGS_DEBUG "-s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -flto -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")

else()
    target_include_directories(Runner PRIVATE ${SDL2_INCLUDE_DIRS} ../../include ../../glm ../../yaml-cpp/include)
    if (WIN32)
        target_link_libraries(Runner PRIVATE yaml-cpp SDL2::SDL2main SDL2::SDL2-static )
    else()
        target_link_libraries(Runner PRIVATE yaml-cpp ${SDL2_LIBRARIES})
    endif (WIN32)	
endif()