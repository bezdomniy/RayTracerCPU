file(GLOB SOURCES
    *.h
    *.cpp
	../../simplexNoise/*.h
	../../simplexNoise/*.cpp
)

add_executable(RayTracer ${SOURCES})

# if(${TO_NATIVE})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "--preload-file ../scenes@/ --use-preload-plugins -s EXPORTED_FUNCTIONS='[_processScene, _renderScene]' -s BUILD_AS_WORKER=1 -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "--preload-file ../scenes@/ --use-preload-plugins -s EXPORTED_FUNCTIONS='[_processScene, _renderScene]' -s BUILD_AS_WORKER=1 -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	target_link_libraries(RayTracer PRIVATE glm yaml-cpp)

	target_include_directories(RayTracer PRIVATE ../../include)
	
else()
	target_include_directories(RayTracer PRIVATE ${SDL2_INCLUDE_DIRS} ../../include)
	if (WIN32)
		target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb SDL2::SDL2main SDL2::SDL2-static)
	else()
		target_link_libraries(RayTracer PRIVATE ${GLM_LIBRARIES} tbb yaml-cpp ${SDL2_LIBRARIES} )
	endif (WIN32)	
endif()
