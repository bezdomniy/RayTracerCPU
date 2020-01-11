#ifdef __EMSCRIPTEN__

#include "emscriptenRunner.h"

using namespace emscripten;

EmscriptenRunner::EmscriptenRunner()
{
    this->sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
    this->halfSubPixelSize = 1.f / (float)(this->sqrtRaysPerPixel) / 2.f;
}

EmscriptenRunner::~EmscriptenRunner()
{
}

void EmscriptenRunner::init(const std::string &sceneDesc)
{
    this->camera = this->world.loadFromFile(sceneDesc);

    this->renderer = Renderer(this->camera);

    if (!done())
        this->pixelsToRender.clear();

    this->pixelsToRender.reserve(this->camera->vsize * this->camera->hsize);

    for (int y = 0; y < this->camera->vsize; y++)
    {
        for (int x = 0; x < this->camera->hsize; x++)
        {
            this->pixelsToRender.push_back(std::make_pair(x, y));
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(), g);
}

emscripten::val EmscriptenRunner::renderToRGBA()
{
    std::vector<uint8_t> byteBuffer;
    size_t bufferLength;

    for (int i = 0; i < PIXELS_PER_BATCH; i++)
    {
        if (!done())
        {
            this->renderer.renderPixel(this->world, this->pixelsToRender.back(), this->sqrtRaysPerPixel, this->halfSubPixelSize);
            this->pixelsToRender.pop_back();
        }
    }

    std::tie(byteBuffer, bufferLength) = this->renderer.canvas.writeToRGBA(false);

    return emscripten::val(emscripten::typed_memory_view(bufferLength, &byteBuffer[0]));
}

bool EmscriptenRunner::done()
{
    return this->pixelsToRender.empty();
}

int EmscriptenRunner::getHeight()
{
    return this->camera->vsize;
}
int EmscriptenRunner::getWidth()
{
    return this->camera->hsize;
}

#endif