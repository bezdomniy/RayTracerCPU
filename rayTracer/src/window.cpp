#include "window.h"
#include <iostream>

Window::Window(const std::string &sceneDesc)
{
    this->sceneDesc = sceneDesc;
    initWindow();
    


}


Window::~Window()
{
}


void Window::initWindow() {
    this->camera = this->world.loadFromFile(this->sceneDesc);
    this->rayTraceRenderer = Renderer(this->camera);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(this->camera->hsize, this->camera->vsize, 0, &this->window, &this->renderer);

    this->running = true;
    this->somethingChanged = true;
}

void Window::run() {
    while (this->running) {
        handleEvents();

        if (this->somethingChanged) {
            update();
            draw();
            this->somethingChanged = false;
        }
    }
}

void Window::handleEvents() {

    SDL_WaitEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        this->running = false;
        break;
    default:
        break;
    }

}

void Window::update() {
    this->rayTraceRenderer.render(world);
    this->somethingChanged = false;
}

void Window::draw() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);

    for (int i = 0; i < this->camera->hsize; i++)
    {
      for (int j = 0; j < this->camera->vsize; j++)
      {
        glm::ivec3 colour(this->rayTraceRenderer.canvas.getPixelInt(i,j));
        SDL_SetRenderDrawColor(this->renderer, colour.x, colour.y, colour.z, 255);
        SDL_RenderDrawPoint(this->renderer, i,j);
      }
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(1000/30);
}