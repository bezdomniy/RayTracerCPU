#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <vector>
#include <cmath>

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
    // static const int PIXELS_PER_BATCH = 20000;
    // const double STEP_SIZE = 0.05f;

    SDL_Event event;

    int width = 300;
    int height = 100;

    // worker_handle sceneProcessWorker;

    // std::string sceneDesc;
    // std::shared_ptr<Camera> camera;

        // Renderer rayTraceRenderer;

    // std::shared_ptr<World> world;
    // std::vector<uint8_t> byteBuffer;
    // size_t bufferLength;

    void handleEvents();

    // void moveCamera(double posChange, glm::dvec3 axis);

    // void _drawTest();

public:
    Window();
    Window(const std::string &sceneDesc);
    // Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world);
    ~Window();

    bool running = false;
    bool somethingChanged = false;

    // void moveLeft();
    // void moveRight();
    // void moveUp();
    // void moveDown();

    char *sceneBinary;
    int sceneSize = 0;

    char *pixelsBinary;
    int pixelsSize = 0;

    void initWindow();
    void update();
    void draw();
    void step();
    void run();
};
