#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#else
#include <SDL.h>
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
#else
    Vec4 originalCameraPosition;
    std::shared_ptr<Camera> camera;
    Renderer rayTraceRenderer;
    std::shared_ptr<World> world;
#endif
    // static const int PIXELS_PER_BATCH = 20000;
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    const Float STEP_SIZE = 0.01f;

    // std::string sceneDesc;
    // std::vector<uint8_t> byteBuffer;
    // size_t bufferLength;

    void handleEvents();

    void moveCamera(Float posChange, uint8_t axis);

    // void _drawTest();

public:
    Window();

    // big assumption that sizeof(float) == sizeof(int)
    const size_t offset = (sizeof(int) * 2) + (sizeof(char) * 2);
    int nWorkers;

    int width;
    int height;

    Float xRotation = 0.0f;
    Float yRotation = 0.0f;

#ifndef __EMSCRIPTEN__
    Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
    Window(const std::string &sceneDesc);
#endif
    ~Window();

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

    void initWindow();

    void update();

    void draw();
    void step();
    void run();
};
