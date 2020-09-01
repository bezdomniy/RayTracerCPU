#include "window.h"

// #include <chrono>
// #include <thread>

Window::Window()
{
    this->nWorkers = 1;

    const auto processor_count = std::thread::hardware_concurrency();

    if (processor_count > 0)
        this->nWorkers = processor_count;
}

#ifdef __EMSCRIPTEN__
void Window::processScene(const std::string &sceneDesc)
{
    for (int i = 0; i < this->nWorkers; ++i)
    {
        this->workers.push_back(emscripten_create_worker("RayTracer.wasm.js"));
    }

    emscripten_call_worker(this->workers.at(0), "processScene", const_cast<char *>(sceneDesc.c_str()), sceneDesc.length(), processCback, (void *)42);

    // this->sceneDesc = sceneDesc;

    // ObjectLoader objectLoader;
    // std::tie(this->camera, this->world) = objectLoader.loadYaml(sceneDesc);

    // this->rayTraceRenderer = Renderer(this->camera);

    // _drawTest();
}

void Window::destroyProcessorWorker()
{
    emscripten_destroy_worker(this->workers.at(0));
}
#endif

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
    // SDL_CreateWindowAndRenderer(this->width, this->height, 0, &this->window, &this->renderer);

    this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, this->width, this->height, 0);
    this->renderer = SDL_CreateRenderer(this->window, -1,
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    this->texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET, this->width, this->height);

    // this->running = true;
}

void Window::step()
{
    handleEvents();

    // TODO only start updating if not already running - otherwise you will get runtime errors
    if (somethingChanged)
    {
        somethingChanged = false;
        running = true;
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
    while (SDL_PollEvent(&event))
    {
        // SDL_WaitEvent(&event);
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
                break;
            case SDLK_RIGHT:
                moveRight();
                break;
            case SDLK_UP:
                moveUp();
                break;
            case SDLK_DOWN:
                moveDown();
                break;
                // cases for other keypresses
            }
            break;
        default:
            break;
        }
    }
}

void Window::moveLeft() { moveCamera(STEP_SIZE, 1); }
void Window::moveRight() { moveCamera(-STEP_SIZE, 1); }
void Window::moveUp() { moveCamera(-STEP_SIZE, 0); }
void Window::moveDown() { moveCamera(STEP_SIZE, 0); }

void Window::moveCamera(float posChange, uint8_t axis)
{
    if (axis == 0)
    {
        this->xRotation += posChange;
    }
    else if (axis == 1)
    {
        this->yRotation += posChange;
    }

    this->somethingChanged = true;

    // // old from here - move to raytracer module
    // glm::dmat4 rotationY =
    //     glm::rotate(glm::dmat4(1.0), posChange, axis);

    // // glm::dmat4 rotationZ =
    // //     glm::rotate(glm::dmat4(1.0), posChange,
    // //                 glm::dvec3(0.0, 0.0, 1.0));

    // this->camera->position = rotationY * this->camera->position;

    // // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

    // // this->camera->position.x += posChange;
    // this->camera->updateTransform();
}

std::vector<uint8_t> floatToByteArray(float d)
{
    uint8_t *bytePointer = reinterpret_cast<uint8_t *>(&d);
    return std::vector<uint8_t>(bytePointer, bytePointer + sizeof(float));

    // union {
    //     float d;
    //     int32_t i;
    // } n;

    // n.d = d;

    // std::vector<uint8_t> bytes(4);
    // bytes[0] = (n.i >> 24) & 0xFF;
    // bytes[1] = (n.i >> 16) & 0xFF;
    // bytes[2] = (n.i >> 8) & 0xFF;
    // bytes[3] = n.i & 0xFF;

    // return bytes;
}

void Window::update()
{
    // this->rayTraceRenderer.render(*world);
    // std::cout << "update?" << std::endl;
    // if (somethingChanged && !running)
    // {

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);

    std::cout << "update" << std::endl;
    this->running = true;

    int i = 0;
    for (auto &renderWorker : this->workers)
    {
        // rotation info

        std::cout << "Send rotations: " << this->xRotation << " " << this->yRotation << std::endl;

        std::vector<uint8_t> xRotationBytes = floatToByteArray(this->xRotation);
        std::vector<uint8_t> yRotationBytes = floatToByteArray(this->yRotation);

        // TODO implement rotation based on this in the renderer api
        for (auto &byte : xRotationBytes)
            this->sceneBinary.push_back(byte);

        for (auto &byte : yRotationBytes)
            this->sceneBinary.push_back(byte);

        this->sceneBinary.push_back((char)i);
        this->sceneBinary.push_back((char)this->nWorkers);

        // TODO fix ints to sizes of floats
        float *xRotationp = reinterpret_cast<float *>(&this->sceneBinary[0] + this->sceneBinary.size() - 10);
        float *yRotationp = reinterpret_cast<float *>(&this->sceneBinary[0] + this->sceneBinary.size() - 6);

        std::cout << "Receive Rotations: " << *xRotationp << " " << *yRotationp << std::endl;

        emscripten_call_worker(renderWorker, "renderScene", &this->sceneBinary[0], this->sceneBinary.size(), renderCback, (void *)42);

        i++;

        // this->somethingChanged = false;
    }
}

void Window::draw(uint8_t workerId)
{
    int pixelsOffset;

    SDL_SetRenderTarget(this->renderer, this->texTarget);

    // std::cout << workerId << " " << this->nWorkers << std::endl;

    for (int j = 0; j < this->height; j++)
    {
        if (j % this->nWorkers == workerId)
        {
            // std::cout << "Writing row: " << j << std::endl;
            for (int i = 0; i < this->width; i++)
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