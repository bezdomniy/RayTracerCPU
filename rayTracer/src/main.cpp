#include "camera.h"
#include "renderer.h"
#include "objectLoader.h"
#include "world.h"
#include "serialisation.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "emscriptenRunner.h"
using namespace emscripten;
#endif

#include <sstream>
void renderToPPM(const std::string &sceneDesc)
{
  // std::stringstream ss;

  // {
  //   ObjectLoader objectLoader;
  //   std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> ret = objectLoader.loadYaml(sceneDesc);

  //   cereal::BinaryOutputArchive oarchive(ss);
  //   oarchive(ret.first, ret.second);
  // }

  // std::shared_ptr<Camera> camera;
  // std::shared_ptr<World> world;

  // cereal::BinaryInputArchive iarchive(ss);
  // iarchive(camera, world);

  // Renderer renderer(camera);
  // renderer.render(*world);

  ObjectLoader objectLoader;
  std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> ret = objectLoader.loadYaml(sceneDesc);
  Renderer renderer(ret.first);
  renderer.render(*ret.second);

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
