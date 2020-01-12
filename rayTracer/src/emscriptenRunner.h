#ifdef __EMSCRIPTEN__

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <algorithm>
#include <random>

#include <glm/glm.hpp>

#include <emscripten.h>
#include <emscripten/val.h>

#include "camera.h"
#include "renderer.h"
#include "world.h"
#include "objectLoader.h"

class EmscriptenRunner
{
private:
    static const int PIXELS_PER_BATCH = 40000;
    World world;
    std::shared_ptr<Camera> camera;
    Renderer renderer;
    std::vector<std::pair<int, int>> pixelsToRender;

    int sqrtRaysPerPixel;
    float halfSubPixelSize;
    const float STEP_SIZE = 0.05f;

    std::random_device rd;
    std::mt19937 g;

    void moveCamera(float posChange);

public:
    EmscriptenRunner(/* args */);
    void init(const std::string &sceneDesc);
    ~EmscriptenRunner();

    emscripten::val renderToRGBA();
    int getHeight();
    int getWidth();
    void moveLeft();
    void moveRight();
    bool done();
};

#endif