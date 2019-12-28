#include "camera.h"
#include "canvas.h"
#include "geometry.h"
#include "pattern.h"
#include "patterns.h"
#include "plane.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

int main(int argc, char const *argv[])
{
    World world;
    std::shared_ptr<Camera> camera = world.loadFromFile("./src/scene2.yaml");
    // std::shared_ptr<Camera> camera = world.loadFromFile("scene2.yaml");

    Renderer renderer(camera);
    renderer.render(world);
    renderer.canvas.writeToPPM("./test.ppm", false);

    #ifdef __EMSCRIPTEN__
    emscripten::val::global("window").call<void>(
        "offerFileAsDownload",
        std::string("test.ppm"),
        std::string("mime/type")
  );
    #endif
    return 0;
}
