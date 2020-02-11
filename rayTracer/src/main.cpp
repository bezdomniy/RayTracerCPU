#include "camera.h"
#include "renderer.h"
#include "world.h"
#include "window.h"
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanInitialiser.h"

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "emscriptenRunner.h"
using namespace emscripten;
#endif

#include "group.h"

void renderToPPM(const std::string &sceneDesc)
{
  World world;
  std::shared_ptr<Camera> camera = world.loadFromFile(sceneDesc);

  Renderer renderer(camera);
  renderer.render(world);
  renderer.canvas.writeToPPM("out.ppm", false);
}

void renderToSDL(const std::string &sceneDesc)
{
  Window window(sceneDesc);
  window.run();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Module)
{
  class_<EmscriptenRunner>("EmscriptenRunner")
      .constructor()
      .function("init", &EmscriptenRunner::init)
      .function("renderToRGBA", &EmscriptenRunner::renderToRGBA)
      .function("done", &EmscriptenRunner::done)
      .function("moveLeft", &EmscriptenRunner::moveLeft)
      .function("moveRight", &EmscriptenRunner::moveRight)
      .function("getHeight", &EmscriptenRunner::getHeight)
      .function("getWidth", &EmscriptenRunner::getWidth);
}
#else

int main(int argc, char const *argv[])
{
  // renderToSDL("scenes/shadowPuppets.yaml");
  // renderToPPM("scenes/reflectionScene.yaml");
   //VulkanApp vulkanApp;
   //vulkanApp.initWindow();
   //vulkanApp.initVulkan();


  World world;
  std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(20., 0., -35., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 400, 200,  0.524);

  std::shared_ptr<Group> group = std::make_shared<Group>();

  std::shared_ptr<Shape> sphere = std::make_shared<Sphere>();
  glm::dmat4 scale =
          glm::scale(glm::dmat4(1.0), glm::dvec3(3.,3.,3.));
  sphere->multiplyTransform(scale);

  std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
        glm::dvec4(glm::dvec4(10.,10.,10.,1.)),
        glm::dvec3(1.,1.,1.));


  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.1,0.1,1.);
  sphere->setMaterial(material);

  group->addChild(sphere);
  std::shared_ptr<Shape> groupShape = std::move(std::dynamic_pointer_cast<Shape>(group));

  // TODO add local_intersect function to group to make this work
  world.addShape(groupShape);
  world.addLight(light);

  Renderer renderer(camera);
  renderer.render(world);
  renderer.canvas.writeToPPM("out.ppm", false);



  return 0;
}
#endif
