#include "window.h"

// #include <chrono>
// #include <thread>

#ifdef __EMSCRIPTEN__
Window::Window()
{
    this->nWorkers = 1;

#ifndef WITH_THREADS
    // const auto processor_count = std::thread::hardware_concurrency();

    // if (processor_count > 0)
    // {
    //     // if (processor_count > 6)
    //     //     this->nWorkers = 6;
    //     // else
    //     this->nWorkers = processor_count;
    // }
#endif //WITH_THREADS
}

void Window::processScene(const std::string &sceneDesc)
{
    if (workers.size() < this->nWorkers)
    {
        for (int i = workers.size(); i < this->nWorkers; i++)
        {
            addWorker();
        }
    }
    else
    {
        for (int i = this->nWorkers; i < workers.size(); i++)
        {
            killWorker();
        }
    }

    emscripten_call_worker(this->workers.at(0), "processScene", const_cast<char *>(sceneDesc.c_str()), sceneDesc.length(), processCback, (void *)42);

    // this->sceneDesc = sceneDesc;

    // ObjectLoader objectLoader;
    // std::tie(this->camera, this->world) = objectLoader.loadYaml(sceneDesc);

    // this->rayTraceRenderer = Renderer(this->camera);

    // _drawTest();
}

void Window::killWorker()
{
    if (auto &worker = workers.back())
    {
        emscripten_destroy_worker(worker);
        workers.pop_back();
        busyWorkers.pop_back();
    }
}

void Window::addWorker()
{
    this->workers.push_back(emscripten_create_worker("/js/RayTracer.wasm.js"));
    this->busyWorkers.push_back(false);
}

void Window::updateSize()
{
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);

    SDL_SetWindowSize(window, this->width, this->height);
    SDL_DestroyTexture(this->texTarget);
    this->texTarget = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET, this->width, this->height);
}

void Window::draw(uint8_t workerId)
{
    int pixelsOffset;

    SDL_SetRenderTarget(this->renderer, this->texTarget);

    // std::cout << workerId << " " << this->nWorkers << std::endl;

    for (int j = 0; j < this->height; j++)
    {
        // if (true)
        if (j % this->nWorkers == workerId)
        {
            // std::cout << "Writing row: " << j << std::endl;
            for (int i = 0; i < this->width; i++)
            {
                // glm::ivec3 colour(this->rayTraceRenderer.canvas.getPixelInt(i, j));
                // if (this->height >= this->width)
                pixelsOffset = (((j / this->nWorkers) * this->width) + i) * 4;
                // else
                //     pixelsOffset = (i * this->height + j) * 4;

                // SDL_SetRenderDrawColor(this->renderer,
                //                        this->pixelsBinary.at(pixelsOffset),
                //                        this->pixelsBinary.at(pixelsOffset + 1),
                //                        this->pixelsBinary.at(pixelsOffset + 2),
                //                        255);

                uint8_t *x = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(workerId).at(pixelsOffset)));
                uint8_t *y = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(workerId).at(pixelsOffset + 1)));
                uint8_t *z = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(workerId).at(pixelsOffset + 2)));
                uint8_t *w = reinterpret_cast<uint8_t *>(&(this->pixelsBinary.at(workerId).at(pixelsOffset + 3)));

                SDL_SetRenderDrawColor(this->renderer, *x, *y, *z, *w);
                // SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);

                SDL_RenderDrawPoint(this->renderer, i, j);
            }
        }
    }

    SDL_SetRenderTarget(this->renderer, NULL);
    SDL_RenderCopy(this->renderer, this->texTarget, NULL, NULL);

    SDL_RenderPresent(this->renderer);

    // SDL_Delay(1000/30);
}
#else
Window::Window(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world)
{
    this->camera = camera;
    this->world = world;
    this->rayTraceRenderer = Renderer(this->camera);

    this->originalCameraPosition = this->camera->position;

    initWindow();
}

Window::Window(const std::string &sceneDesc)
{
    ObjectLoader objectLoader;
    std::tie(this->camera, this->world) = objectLoader.loadYaml(sceneDesc);
    this->rayTraceRenderer = Renderer(this->camera);

    this->originalCameraPosition = this->camera->position;

    initWindow();
}
#endif
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

#ifdef __EMSCRIPTEN__
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
#ifdef __EMSCRIPTEN__
    this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, this->width, this->height, 0);
#else
    this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, this->camera->hsize, this->camera->vsize, 0);
#endif
    this->renderer = SDL_CreateRenderer(this->window, -1,
                                        SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);

    // SDL_CreateWindowAndRenderer(this->camera->hsize, this->camera->vsize, 0, &this->window, &this->renderer);

#ifdef __EMSCRIPTEN__
    this->texTarget = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET, this->width, this->height);
#endif

    // this->running = true;
    this->initialised = true;
    this->somethingChanged = true;

    std::cout << "Created window." << std::endl;
}

void Window::step()
{
    handleEvents();

    // TODO only start updating if not already running - otherwise you will get runtime errors

    if (somethingChanged && !running)
    {
        somethingChanged = false;
        update();
#ifndef __EMSCRIPTEN__
        draw();
#endif
    }
}

void Window::run()
{
    while (this->initialised)
    {
        step();
    }
}

void Window::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            this->initialised = false;
            break;
        }
    }

    if (keystate[SDL_SCANCODE_ESCAPE])
    {
        this->initialised = false;
    }
    if (keystate[SDL_SCANCODE_LEFT])
    {
        moveLeft();
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        moveRight();
    }
    if (keystate[SDL_SCANCODE_UP])
    {
        moveUp();
    }
    if (keystate[SDL_SCANCODE_DOWN])
    {
        moveDown();
    }

    // while (SDL_PollEvent(&event))
    // {
    //     // SDL_WaitEvent(&event);
    //     switch (event.type)
    //     {
    //     case SDL_QUIT:
    //         this->initialised = false;
    //         break;
    //     case SDL_KEYDOWN:
    //         switch (event.key.keysym.sym)
    //         {
    //         case SDLK_ESCAPE:
    //             this->initialised = false;
    //             break;
    //         case SDLK_LEFT:
    //             moveLeft();
    //             break;
    //         case SDLK_RIGHT:
    //             moveRight();
    //             break;
    //         case SDLK_UP:
    //             moveUp();
    //             break;
    //         case SDLK_DOWN:
    //             moveDown();
    //             break;
    //             // cases for other keypresses
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    // }
}

void Window::moveLeft() { moveCamera(STEP_SIZE, 1); }
void Window::moveRight() { moveCamera(-STEP_SIZE, 1); }
void Window::moveUp() { moveCamera(-STEP_SIZE, 0); }
void Window::moveDown() { moveCamera(STEP_SIZE, 0); }

void Window::moveCamera(Float posChange, uint8_t axis)
{
    if (axis == 0)
    {
        this->xRotation += posChange;
    }
    else if (axis == 1)
    {
        this->yRotation += posChange;
    }

#ifndef __EMSCRIPTEN__
    Mat4 rotationX =
        glm::rotate(Mat4(1.0), (Float)this->xRotation, Vec3(1.0, 0.0, 0.0));

    Mat4 rotationY =
        glm::rotate(Mat4(1.0), (Float)this->yRotation, Vec3(0.0, 1.0, 0.0));

    // Mat4 rotationZ =
    //     glm::rotate(Mat4(1.0), posChange,
    //                 Vec3(0.0, 0.0, 1.0));

    this->camera->position = rotationX * this->originalCameraPosition;
    this->camera->position = rotationY * this->camera->position;
    this->camera->updateTransform();
#endif

    this->somethingChanged = true;

    // // old from here - move to raytracer module
    // Mat4 rotationY =
    //     glm::rotate(Mat4(1.0), posChange, axis);

    // // Mat4 rotationZ =
    // //     glm::rotate(Mat4(1.0), posChange,
    // //                 Vec3(0.0, 0.0, 1.0));

    // this->camera->position = rotationY * this->camera->position;

    // // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

    // // this->camera->position.x += posChange;
    // this->camera->updateTransform();
}

std::vector<uint8_t> floatToByteArray(Float d)
{
    uint8_t *bytePointer = reinterpret_cast<uint8_t *>(&d);
    return std::vector<uint8_t>(bytePointer, bytePointer + sizeof(Float));
}

void Window::update()
{

    // std::cout << "update?" << std::endl;
    // if (somethingChanged && !running)
    // {

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);
    // std::cout << "update" << std::endl;
    this->running = true;

#ifdef __EMSCRIPTEN__

    // std::cout << "Send rotations: " << this->xRotation << " " << this->yRotation << std::endl;

    std::vector<uint8_t> xRotationBytes = floatToByteArray(this->xRotation);
    std::vector<uint8_t> yRotationBytes = floatToByteArray(this->yRotation);

    // TODO implement rotation based on this in the renderer api
    int i = this->sceneBinary.size() - ((sizeof(int) * 2) + (sizeof(char) * 2));
    for (auto &byte : xRotationBytes)
    {
        this->sceneBinary[i] = byte;
        i++;
    }

    for (auto &byte : yRotationBytes)
    {
        this->sceneBinary[i] = byte;
        i++;
    }

    // this->sceneBinary.push_back((char)this->nWorkers);
    this->sceneBinary[i] = ((char)this->nWorkers);
    i++;

    // std::cout << "nworkers " << (int)nWorkers << std::endl;

    // TODO fix ints to sizes of floats
    // Float *xRotationp = reinterpret_cast<Float *>(&this->sceneBinary[0] + this->sceneBinary.size() - 10);
    // Float *yRotationp = reinterpret_cast<Float *>(&this->sceneBinary[0] + this->sceneBinary.size() - 6);

    int worker = 0;
    for (auto &renderWorker : this->workers)
    {
        // rotation info

        this->busyWorkers.at(worker) = true;
        this->sceneBinary[i] = ((char)worker);
#ifdef WITH_THREADS
        emscripten_call_worker(renderWorker, "renderSceneThreaded", &this->sceneBinary[0], this->sceneBinary.size(), renderCback, (void *)42);
#else
        emscripten_call_worker(renderWorker, "renderScene", &this->sceneBinary[0], this->sceneBinary.size(), renderCback, (void *)42);
#endif //WITH_THREADS \
    // std::cout << "Receive Rotations: " << *xRotationp << " " << *yRotationp << std::endl;
        worker++;
        // this->somethingChanged = false;
    }

#else
    this->rayTraceRenderer.render(*world);
    this->running = false;
#endif //__EMSCRIPTEN__
}

#ifndef __EMSCRIPTEN__
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

    SDL_RenderPresent(this->renderer);

    // SDL_Delay(1000/30);
}
#endif
