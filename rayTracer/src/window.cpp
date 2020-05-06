#include "window.h"
#include <iostream>

Window::Window(const std::string &sceneDesc)
{
    this->sceneDesc = sceneDesc;
    initWindow();
    // _drawTest();
}

Window::~Window()
{
}

void Window::_drawTest()
{
    this->camera = this->world.loadFromFile(this->sceneDesc);
    this->rayTraceRenderer = Renderer(this->camera);
    update();
}

void Window::initWindow()
{
    this->camera = this->world.loadFromFile(this->sceneDesc);
    this->rayTraceRenderer = Renderer(this->camera);

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(this->camera->hsize, this->camera->vsize, 0, &this->window, &this->renderer);

    this->running = true;
    this->somethingChanged = true;
}

void Window::run()
{
    while (this->running)
    {
        handleEvents();

        if (this->somethingChanged)
        {
            update();
            draw();
            this->somethingChanged = false;
        }
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
            moveLeft();
            this->somethingChanged = true;
            break;
        case SDLK_RIGHT:
            moveRight();
            this->somethingChanged = true;
            break;
        case SDLK_UP:
            moveUp();
            this->somethingChanged = true;
            break;
        case SDLK_DOWN:
            moveDown();
            this->somethingChanged = true;
            break;
            // cases for other keypresses
        }
        break;
    default:
        break;
    }
}

void Window::moveLeft() { moveCamera(STEP_SIZE, glm::dvec3(0.0, 1.0, 0.0)); }
void Window::moveRight() { moveCamera(-STEP_SIZE, glm::dvec3(0.0, 1.0, 0.0)); }
void Window::moveUp() { moveCamera(-STEP_SIZE, glm::dvec3(1.0, 0.0, 0.0)); }
void Window::moveDown() { moveCamera(STEP_SIZE, glm::dvec3(1.0, 0.0, 0.0)); }


void Window::moveCamera(double posChange, glm::dvec3 axis)
{
    glm::dmat4 rotationY =
        glm::rotate(glm::dmat4(1.0), posChange, axis);

    // glm::dmat4 rotationZ =
    //     glm::rotate(glm::dmat4(1.0), posChange,
    //                 glm::dvec3(0.0, 0.0, 1.0));

    this->camera->position = rotationY * this->camera->position;

    // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

    // this->camera->position.x += posChange;
    this->camera->updateTransform();
}

void Window::update()
{
    this->rayTraceRenderer.render(world);
    this->somethingChanged = false;
}

void Window::draw()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);

    for (int i = 0; i < this->camera->hsize; i++)
    {
        for (int j = 0; j < this->camera->vsize; j++)
        {
            glm::ivec3 colour(this->rayTraceRenderer.canvas.getPixelInt(i, j));
            SDL_SetRenderDrawColor(this->renderer, colour.x, colour.y, colour.z, 255);
            SDL_RenderDrawPoint(this->renderer, i, j);
        }
    }

    SDL_RenderPresent(renderer);

    // SDL_Delay(1000/30);
}