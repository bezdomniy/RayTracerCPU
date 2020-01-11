#ifdef __EMSCRIPTEN__

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <algorithm>
#include <random>

#include <emscripten.h>
#include <emscripten/val.h>

#include "camera.h"
#include "renderer.h"
#include "world.h"
#include "objectLoader.h"

class EmscriptenRunner
{
private:
    static const int PIXELS_PER_BATCH = 1000;
    World world;
    std::shared_ptr<Camera> camera;
    Renderer renderer;
    std::vector<std::pair<int, int>> pixelsToRender;

    int sqrtRaysPerPixel;
    float halfSubPixelSize;

public:
    EmscriptenRunner(/* args */);
    void init(const std::string &sceneDesc);
    ~EmscriptenRunner();

    emscripten::val renderToRGBA();
    int getHeight();
    int getWidth();
    bool done();
};

#endif