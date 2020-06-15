#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <cmath>

#include "camera.h"
#include "renderer.h"
#include "world.h"

class Window
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    static const int PIXELS_PER_BATCH = 20000;
    const double STEP_SIZE = 0.05f;

    SDL_Event event;
    bool running = false;

    int width;
    int height;

    std::string sceneDesc;
    std::shared_ptr<Camera> camera;

    bool somethingChanged = false;

    Renderer rayTraceRenderer;

    std::shared_ptr<World> world;
    std::vector<uint8_t> byteBuffer;
    size_t bufferLength;

    void handleEvents();

    void moveCamera(double posChange, glm::dvec3 axis);

    void _drawTest();

public:
    Window(const std::string &sceneDesc);
    Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
    ~Window();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void initWindow();
    void update();
    void draw();
    void run();
};
