#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <cmath>

#include <emscripten.h>
#include <iostream>

#include <unordered_map>

#include <thread>

// #include "camera.h"
// #include "renderer.h"
// #include "world.h"
// #include "objectLoader.h"

void processCback(char *data, int size, void *arg);
void renderCback(char *data, int size, void *arg);

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texTarget;
    // static const int PIXELS_PER_BATCH = 20000;
    const double STEP_SIZE = 0.05f;

    SDL_Event event;

    worker_handle sceneProcessWorker;

    int nWorkers;

    int32_t xRotation = 0;
    int32_t yRotation = 0;

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
    void processScene(const std::string &sceneDesc);
    // Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
    ~Window();

    bool running = false;
    bool somethingChanged = false;

    int width;
    int height;

    void destroyProcessorWorker();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    std::vector<char> sceneBinary;

    std::unordered_map<uint8_t, std::vector<char>> pixelsBinary;

    void initWindow();
    void update();
    void draw(uint8_t workerId);
    void step();
    void run();
};
