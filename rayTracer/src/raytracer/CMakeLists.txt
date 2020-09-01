file(GLOB SOURCES
    *.h
    *.cpp
	../../simplexNoise/*.h
	../../simplexNoise/*.cpp
)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	add_executable(RayTracer ${SOURCES})
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "--preload-file ../assets@/  --use-preload-plugins -s EXPORTED_FUNCTIONS='[_processScene, _renderScene]' -s BUILD_AS_WORKER=1 -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "--preload-file ../assets@/  --use-preload-plugins -s EXPORTED_FUNCTIONS='[_processScene, _renderScene]' -s BUILD_AS_WORKER=1 -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	target_link_libraries(RayTracer PRIVATE glm yaml-cpp)

	target_include_directories(RayTracer PRIVATE ../../include)
	
else()
	add_library(RayTracer ${SOURCES})
	target_include_directories(RayTracer PRIVATE ../../include)
	if (WIN32)
		target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb)
	else()
		target_link_libraries(RayTracer PRIVATE ${GLM_LIBRARIES} tbb yaml-cpp )
	endif (WIN32)	
endif()
