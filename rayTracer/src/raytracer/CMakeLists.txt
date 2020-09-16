file(GLOB SOURCES
    *.h
    *.cpp
	../../simplexNoise/*.h
	../../simplexNoise/*.cpp
)



if (${USE_THREADS})
	add_compile_definitions(WITH_THREADS)
endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	add_executable(RayTracer ${SOURCES})
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FETCH=1" )
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "-s EXPORTED_FUNCTIONS='[_processScene, _renderScene _renderSceneThreaded]' -flto -s BUILD_AS_WORKER=1 -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "-s EXPORTED_FUNCTIONS='[_processScene, _renderScene _renderSceneThreaded]' -flto -s BUILD_AS_WORKER=1 -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	# --preload-file ../../../assets@/  
	target_link_libraries(RayTracer PRIVATE yaml-cpp)

	if (${USE_THREADS})
		# target_link_libraries(RayTracer PRIVATE cpp-taskflow)
		target_include_directories(RayTracer PRIVATE ../../cpp-taskflow)
	endif()

	target_include_directories(RayTracer PRIVATE ../../glm ../../include)
	
else()
	add_library(RayTracer ${SOURCES})
	target_include_directories(RayTracer PRIVATE ../../include)
	if (WIN32)
		target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb)
	else()
		target_link_libraries(RayTracer PRIVATE ${GLM_LIBRARIES} tbb yaml-cpp )
	endif (WIN32)	
endif()
