file(GLOB SOURCES
    *.h
    *.cpp
	../../simplexNoise/*.h
	../../simplexNoise/*.cpp
)

file(GLOB RUNNERSOURCES
	../runner/*.h
	../runner/*.cpp
)

# TODO: use __EMSCRIPTEN_PTHREADS__ instead
# if (${USE_THREADS})
# 	add_compile_definitions(WITH_THREADS)
# endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")

	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FETCH=1" )

	if (${USE_THREADS})	
	    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 " )
		add_executable(RayTracer ${SOURCES} ${RUNNERSOURCES})
		set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "-pthread -s PTHREAD_POOL_SIZE=navigator.hardwareConcurrency -flto -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
		set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "-pthread -s PTHREAD_POOL_SIZE=navigator.hardwareConcurrency -flto -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	else()
		add_executable(RayTracer ${SOURCES} )
		set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "-s EXPORTED_FUNCTIONS='[_processScene, _renderScene, _renderSceneThreaded]' -flto -s BUILD_AS_WORKER=1 -s WASM=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
		set_target_properties(RayTracer PROPERTIES LINK_FLAGS_DEBUG "-s EXPORTED_FUNCTIONS='[_processScene, _renderScene, _renderSceneThreaded]' -flto -s BUILD_AS_WORKER=1 -s WASM=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
	endif()
	# --preload-file ../../../assets@/  
	target_link_libraries(RayTracer PRIVATE yaml-cpp)
	target_include_directories(RayTracer PRIVATE ../../glm ../../include)
	
else()
	add_executable(RayTracer ${SOURCES} ${RUNNERSOURCES})

	# mathfu_configure_flags(RayTracer)

	target_include_directories(RayTracer PRIVATE ${SDL2_INCLUDE_DIRS} ../../include ../../glm ../../yaml-cpp/include)
	if (WIN32)
		# target_link_libraries(RayTracer PRIVATE glm yaml-cpp TBB::tbb)
		target_link_libraries(RayTracer PRIVATE SDL2::SDL2 SDL2::SDL2main yaml-cpp)
	else()
		# target_link_libraries(RayTracer PRIVATE  ${SDL2_LIBRARIES} tbb yaml-cpp pthread)
		target_link_libraries(RayTracer PRIVATE  ${SDL2_LIBRARIES} yaml-cpp pthread)
	endif (WIN32)	

	set_target_properties(RayTracer PROPERTIES LINK_FLAGS_RELEASE "-flto")

endif()
