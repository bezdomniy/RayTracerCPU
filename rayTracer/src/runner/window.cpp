#include "window.h"

// #include <chrono>
// #include <thread>

Window::Window()
{
}

void Window::processScene(const std::string &sceneDesc)
{
    this->sceneProcessWorker = emscripten_create_worker("RayTracer.wasm.js");
    emscripten_call_worker(this->sceneProcessWorker, "processScene", const_cast<char *>(sceneDesc.c_str()), sceneDesc.length(), processCback, (void *)42);

    // this->sceneDesc = sceneDesc;

    // ObjectLoader objectLoader;
    // std::tie(this->camera, this->world) = objectLoader.loadYaml(sceneDesc);

    // this->rayTraceRenderer = Renderer(this->camera);

    // _drawTest();
}

void Window::destroyProcessorWorker()
{
    emscripten_destroy_worker(this->sceneProcessWorker);
}

// Window::Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world)
// {
//     this->camera = camera;
//     this->world = world;
//     this->rayTraceRenderer = Renderer(this->camera);

//     initWindow();
// }

Window::~Window()
{
}

// void Window::_drawTest()
// {
//     std::shared_ptr<World> world;
//     std::shared_ptr<Camera> camera;

//     ObjectLoader objectLoader;
//     std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);
//     this->rayTraceRenderer = Renderer(this->camera);
//     update();
// }

void Window::initWindow()
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(this->width, this->height, 0, &this->window, &this->renderer);

    // this->running = true;
}

void Window::step()
{
    // handleEvents();

    if (this->somethingChanged)
    {
        update();
    }
}

void Window::run()
{
    while (this->running)
    {
        step();
    }
}

void Window::handleEvents()
{

    SDL_WaitEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        this->running = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            this->running = false;
            break;
        case SDLK_LEFT:
            // moveLeft();
            this->somethingChanged = true;
            break;
        case SDLK_RIGHT:
            // moveRight();
            this->somethingChanged = true;
            break;
        case SDLK_UP:
            // moveUp();
            this->somethingChanged = true;
            break;
        case SDLK_DOWN:
            // moveDown();
            this->somethingChanged = true;
            break;
            // cases for other keypresses
        }
        break;
    default:
        break;
    }
}

// void Window::moveLeft() { moveCamera(STEP_SIZE, glm::dvec3(0.0, 1.0, 0.0)); }
// void Window::moveRight() { moveCamera(-STEP_SIZE, glm::dvec3(0.0, 1.0, 0.0)); }
// void Window::moveUp() { moveCamera(-STEP_SIZE, glm::dvec3(1.0, 0.0, 0.0)); }
// void Window::moveDown() { moveCamera(STEP_SIZE, glm::dvec3(1.0, 0.0, 0.0)); }

// void Window::moveCamera(double posChange, glm::dvec3 axis)
// {
//     glm::dmat4 rotationY =
//         glm::rotate(glm::dmat4(1.0), posChange, axis);

//     // glm::dmat4 rotationZ =
//     //     glm::rotate(glm::dmat4(1.0), posChange,
//     //                 glm::dvec3(0.0, 0.0, 1.0));

//     this->camera->position = rotationY * this->camera->position;

//     // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

//     // this->camera->position.x += posChange;
//     this->camera->updateTransform();
// }

void Window::update()
{
    // this->rayTraceRenderer.render(*world);
    // std::cout << "update?" << std::endl;
    // if (somethingChanged && !running)
    // {
    std::cout << "update" << std::endl;
    this->running = true;
    worker_handle renderWorker = emscripten_create_worker("RayTracer.wasm.js");

    emscripten_call_worker(renderWorker, "renderScene", &this->sceneBinary[0], this->sceneBinary.size(), renderCback, (void *)42);
    // }

    // this->somethingChanged = false;
}

void Window::draw()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);
    int pixelsOffset;

    for (int i = 0; i < this->width; i++)
    {
        for (int j = 0; j < this->height; j++)
        {
            // glm::ivec3 colour(this->rayTraceRenderer.canvas.getPixelInt(i, j));
            // if (this->height >= this->width)
            pixelsOffset = (j * this->width + i) * 4;
            // else
            //     pixelsOffset = (i * this->height + j) * 4;

            // SDL_SetRenderDrawColor(this->renderer,
            //                        this->pixelsBinary.at(pixelsOffset),
            //                        this->pixelsBinary.at(pixelsOffset + 1),
            //                        this->pixelsBinary.at(pixelsOffset + 2),
            //                        255);

            uint8_t *x = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(pixelsOffset)));
            uint8_t *y = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(pixelsOffset + 1)));
            uint8_t *z = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(pixelsOffset + 2)));
            uint8_t *w = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(pixelsOffset + 3)));

            SDL_SetRenderDrawColor(this->renderer, *x, *y, *z, *w);

            SDL_RenderDrawPoint(this->renderer, i, j);
        }
    }

    SDL_RenderPresent(renderer);

    // SDL_Delay(1000/30);
}