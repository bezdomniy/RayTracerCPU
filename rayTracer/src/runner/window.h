#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <cmath>

#include <iostream>

#include <unordered_map>

#include <thread>

// #include "camera.h"
// #include "renderer.h"
// #include "world.h"
// #include "objectLoader.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void processCback(char *data, int size, void *arg);
void renderCback(char *data, int size, void *arg);
#endif

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texTarget;
    // static const int PIXELS_PER_BATCH = 20000;
    const float STEP_SIZE = 0.05f;

    SDL_Event event;

    std::vector<worker_handle> workers;

    // std::string sceneDesc;
    // std::shared_ptr<Camera> camera;

    // Renderer rayTraceRenderer;

    // std::shared_ptr<World> world;
    // std::vector<uint8_t> byteBuffer;
    // size_t bufferLength;

    void handleEvents();

    void moveCamera(float posChange, uint8_t axis);

    // void _drawTest();

public:
    Window();

    std::vector<bool> busyWorkers;

    bool initialised = false;
    float xRotation = 0.0f;
    float yRotation = 0.0f;

    // Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
    ~Window();

    bool running = false;
    bool somethingChanged = false;

    int nWorkers;

    int width;
    int height;

#ifdef __EMSCRIPTEN__
    void processScene(const std::string &sceneDesc);
    void destroyProcessorWorker();
    std::vector<char> sceneBinary;
    std::unordered_map<uint8_t, std::vector<char>> pixelsBinary;
#endif

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void initWindow();
    void updateSize();
    void update();
    void draw(uint8_t workerId);
    void step();
    void run();
    void killWorker();
    void addWorker();
};
