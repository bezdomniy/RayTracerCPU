#include "camera.h"
#include "renderer.h"
#include "objectLoader.h"
#include "world.h"
#include "serialisation.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cereal/archives/binary.hpp>

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
  int size;
  std::string processedScene;
  {
    std::stringstream ss;
    ObjectLoader objectLoader;
    std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> ret = objectLoader.loadYaml(sceneDesc);

    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(ret.first, ret.second);

    // ss << std::flush;

    processedScene = ss.str();
    // ss.seekg(0, std::ios::end);
    // size = ss.tellg();
    // ss.seekg(0, std::ios::beg);

    // std::cout << "Processed size: " << size << std::endl;
    // std::cout << processedScene << std::endl;
    // size_t length = tmp.length();
    // cstr = tmp.c_str();
  }

  {
    std::istringstream iss(processedScene);

    // iss.seekg(0, std::ios::end);
    // int size = iss.tellg();
    // iss.seekg(0, std::ios::beg);

    // std::cout << "Loaded size: " << size << std::endl;

    // const std::string &tmp = iss.str();
    // std::cout << tmp << std::endl;

    std::shared_ptr<Camera> camera;
    std::shared_ptr<World> world;

    cereal::BinaryInputArchive iarchive(iss);
    iarchive(camera, world);

    Renderer renderer(camera);
    renderer.render(*world);

    // ObjectLoader objectLoader;
    // std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> ret = objectLoader.loadYaml(sceneDesc);
    // Renderer renderer(ret.first);
    // renderer.render(*ret.second);

    renderer.canvas.writeToPPM("out.ppm", false);
  }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Module)
{
  class_<EmscriptenRunner>("EmscriptenRunner")
      .constructor()
      // .function("init", &EmscriptenRunner::init)
      .function("renderToRGBA", &EmscriptenRunner::renderToRGBA)
      .function("renderToRGBAThreaded", &EmscriptenRunner::renderToRGBAThreaded)
      .function("renderProcessedToRGBA", &EmscriptenRunner::renderProcessedToRGBA)
      .function("processScene", &EmscriptenRunner::processScene)
      .function("done", &EmscriptenRunner::done)
      .function("moveLeft", &EmscriptenRunner::moveLeft)
      .function("moveRight", &EmscriptenRunner::moveRight)
      .function("getHeight", &EmscriptenRunner::getHeight)
      .function("getWidth", &EmscriptenRunner::getWidth);
}

#else

int main(int argc, char const *argv[])
{
  renderToPPM("scenes/coverScene.yaml");

  return 0;
}
#endif
