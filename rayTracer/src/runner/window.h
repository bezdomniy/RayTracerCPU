#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#elif __APPLE__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <cmath>

#include <iostream>

#include <unordered_map>

#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void processCback(char *data, int size, void *arg);
void renderCback(char *data, int size, void *arg);

#ifdef __EMSCRIPTEN_PTHREADS__
#include "../raytracer/camera.h"
#include "../raytracer/renderer.h"
#include "../raytracer/world.h"
#include "../raytracer/objectLoader.h"
#endif //__EMSCRIPTEN_PTHREADS__

#else
#include "../raytracer/camera.h"
#include "../raytracer/renderer.h"
#include "../raytracer/world.h"
#include "../raytracer/objectLoader.h"
#endif

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
#ifdef __EMSCRIPTEN__
    SDL_Texture *texTarget;
    std::vector<worker_handle> workers;
#endif

#ifdef __EMSCRIPTEN_PTHREADS__
    glm::dvec4 originalCameraPosition;
    std::shared_ptr<Camera> camera;
    Renderer rayTraceRenderer;
    std::shared_ptr<World> world;
#endif //__EMSCRIPTEN_PTHREADS__

    // static const int PIXELS_PER_BATCH = 20000;
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    const float STEP_SIZE = 0.01f;

    // std::string sceneDesc;
    // std::vector<uint8_t> byteBuffer;
    // size_t bufferLength;

    void handleEvents();

    void moveCamera(float posChange, uint8_t axis);

    // void _drawTest();

public:
    Window();

    // big assumption that sizeof(float) == sizeof(int)
    const size_t offset = (sizeof(int) * 2) + (sizeof(char) * 2);
    int nWorkers;

    int width;
    int height;

    float xRotation = 0.0f;
    float yRotation = 0.0f;

#ifndef __EMSCRIPTEN__
    Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
#endif
    // Window();
    ~Window();

    void initWindow(const std::string &sceneDesc);

    bool initialised = false;
    bool running = false;
    bool somethingChanged = false;

#ifdef __EMSCRIPTEN__
    std::vector<bool> busyWorkers;
    void processScene(const std::string &sceneDesc);
    std::vector<char> sceneBinary;
    void draw(uint8_t workerId);
    std::unordered_map<uint8_t, std::vector<char>> pixelsBinary;
    void killWorker();
    void addWorker();
    void updateSize();
#endif

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void initSDL();

    void update();

    void draw();
    void step();
    void run();
};
