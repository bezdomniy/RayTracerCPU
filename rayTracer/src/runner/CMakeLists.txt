file(GLOB RUNNERSOURCES
    *.h
    *.cpp
)

add_executable(Runner ${RUNNERSOURCES})
set(CMAKE_EXECUTABLE_SUFFIX ".html")

set_target_properties(Runner PROPERTIES LINK_FLAGS_RELEASE "-s WASM=1 -s USE_SDL=2  -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
set_target_properties(Runner PROPERTIES LINK_FLAGS_DEBUG "-s WASM=1 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s BINARYEN_METHOD='native-wasm' -s ALLOW_MEMORY_GROWTH=1")
