#include "camera.h"
#include "renderer.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "emscriptenRunner.h"
using namespace emscripten;
#endif

void renderToPPM(const std::string &sceneDesc)
{
  World world;
  std::shared_ptr<Camera> camera = world.loadFromFile(sceneDesc);

  Renderer renderer(camera);
  renderer.render(world);
  renderer.canvas.writeToPPM("out.ppm", false);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Module)
{
  class_<EmscriptenRunner>("EmscriptenRunner")
      .constructor()
      // .function("init", &EmscriptenRunner::init)
      .function("renderToRGBA", &EmscriptenRunner::renderToRGBA)
      // .function("done", &EmscriptenRunner::done)
      .function("moveLeft", &EmscriptenRunner::moveLeft)
      .function("moveRight", &EmscriptenRunner::moveRight)
      .function("getHeight", &EmscriptenRunner::getHeight)
      .function("getWidth", &EmscriptenRunner::getWidth);
}
#else

int main(int argc, char const *argv[])
{
  renderToPPM("scenes/groups.yaml");

  // VulkanApp vulkanApp;
  // vulkanApp.initWindow();
  // vulkanApp.initVulkan();

  return 0;
}
#endif
