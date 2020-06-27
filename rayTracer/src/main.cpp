#include "camera.h"
#include "renderer.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

<<<<<<< HEAD
=======
#include "model.h"
#include "plane.h"

// #include "VulkanInitialiser.h"

>>>>>>> 0d43310519f8353236ee83e9f8acc7717a6e27ca
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

<<<<<<< HEAD
=======
void renderToSDL(const std::string &sceneDesc)
{
  Window window(sceneDesc);
  window.run();
}

void renderToSDL(const std::shared_ptr<Camera> &camera, const std::shared_ptr<World> &world)
{
  Window window(camera, world);
  window.run();
}

>>>>>>> 0d43310519f8353236ee83e9f8acc7717a6e27ca
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

<<<<<<< HEAD
int main(int argc, char const *argv[])
{
  renderToPPM("scenes/groups.yaml");

  // VulkanApp vulkanApp;
  // vulkanApp.initWindow();
  // vulkanApp.initVulkan();
=======
std::shared_ptr<Shape> hexagonCorner()
{
  std::shared_ptr<Shape> corner = std::make_shared<Sphere>();
  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.8, 0.8, 0.8);
  corner->setMaterial(material);

  glm::dmat4 scale =
      glm::scale(glm::dmat4(1.0), glm::dvec3(0.25, 0.25, 0.25));

  glm::dmat4 translate =
      glm::translate(glm::dmat4(1.0), glm::dvec3(0., 0., -1.));

  corner->multiplyTransform(scale);
  corner->multiplyTransform(translate);

  corner->calculateInverseTranform();

  return corner;
}

std::shared_ptr<Shape> hexagonEdge()
{
  std::shared_ptr<Shape> edge = std::make_shared<Cylinder>(0., 1., false);
  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.8, 0.8, 0.8);
  edge->setMaterial(material);

  glm::dmat4 translate =
      glm::translate(glm::dmat4(1.0), glm::dvec3(0., 0., -1.));

  glm::dmat4 scale =
      glm::scale(glm::dmat4(1.0), glm::dvec3(0.25, 1., 0.25));

  glm::dmat4 rotationy =
      glm::rotate(glm::dmat4(1.0), 0.52359877559,
                  glm::dvec3(0.0, 1.0, 0.0));

  glm::dmat4 rotationz =
      glm::rotate(glm::dmat4(1.0), 1.57079632679,
                  glm::dvec3(0.0, 0.0, 1.0));

  edge->multiplyTransform(scale);
  edge->multiplyTransform(rotationz);
  edge->multiplyTransform(rotationy);
  edge->multiplyTransform(translate);

  edge->calculateInverseTranform();
  return edge;
}

std::shared_ptr<Group> hexagonSide()
{
  std::shared_ptr<Group> side = std::make_shared<Group>();

  std::shared_ptr<Shape> corner = hexagonCorner();
  std::shared_ptr<Shape> edge = hexagonEdge();

  side->addChild(corner);
  side->addChild(edge);

  return side;
}

std::shared_ptr<Group> hexagon()
{
  std::shared_ptr<Group> hex = std::make_shared<Group>();

  for (int i = 0; i < 6; i++)
  {
    std::shared_ptr<Shape> side = hexagonSide();
    glm::dmat4 rotationy =
        glm::rotate(glm::dmat4(1.0), i * 1.0471975512,
                    glm::dvec3(0.0, 1.0, 0.0));
    side->multiplyTransform(rotationy);

    side->calculateInverseTranform();
    hex->addChild(side);
  }

  return hex;
}

Model mesh(const std::string &objPath)
{
  Model model(objPath);
  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.8, 0.8, 0.8);
  model.mesh->setMaterial(material);

  return model;
}

int main(int argc, char const *argv[])
{
  // renderToSDL("scenes/reflectionScene.yaml");
  // renderToSDL("scenes/coverScene.yaml");
  // renderToSDL("scenes/groups.yaml");
  // renderToSDL("scenes/hippy.yaml");
  // renderToSDL("../../../scenes/hippy.yaml");
  // renderToSDL("scenes/globe.yaml");
  // renderToSDL("scenes/skybox.yaml");
  // renderToSDL("scenes/checkers.yaml");
  // renderToSDL("scenes/christmas.yaml");
  // renderToSDL("scenes/reflectionScene.yaml");
  // renderToSDL("scenes/reflectionScene.yaml");

  // renderToPPM("scenes/christmas.yaml");
  // renderToPPM("scenes/reflectionScene.yaml");

  std::shared_ptr<World> world = std::make_shared<World>();
  std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(-10., 5., 20., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 400, 400, 0.524);

  Model model = mesh("models/armadillo.obj");

  std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
      glm::dvec4(glm::dvec4(10., 10., 10., 1.)),
      glm::dvec3(1., 1., 1.));

  std::shared_ptr<Shape> mesh = std::dynamic_pointer_cast<Shape>(model.mesh);

  // glm::dmat4 scale =
  //     glm::scale(glm::dmat4(1.0), glm::dvec3(3., 3., 3.));
  // mesh->multiplyTransform(scale);
  // mesh->calculateInverseTranform();

  std::shared_ptr<Shape> plane = std::make_shared<Plane>();

  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>();
  planeMaterial->reflective = 0.8;
  planeMaterial->colour = glm::dvec3(0.8, 0.1, 0.1);
  plane->setMaterial(planeMaterial);

  glm::dmat4 translate =
      glm::translate(glm::dmat4(1.0), glm::dvec3(0., -0.2, 0.));
  plane->multiplyTransform(translate);
  plane->calculateInverseTranform();

  world->addShape(mesh);
  // world->addShape(plane);
  world->addLight(light);

  Renderer renderer(camera);
  renderer.render(*world);
  renderer.canvas.writeToPPM("armadillo.ppm", false);

  // renderToSDL(camera, world);

  // std::shared_ptr<World> world = std::make_shared<World>();
  // std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(10., 5., -10., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 400, 200, 0.524);

  // std::shared_ptr<Group> group = hexagon();
  // std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
  //     glm::dvec4(glm::dvec4(10., 10., 10., 1.)),
  //     glm::dvec3(1., 1., 1.));

  // std::shared_ptr<Shape> groupShape = std::move(std::dynamic_pointer_cast<Shape>(group));

  // world->addShape(groupShape);
  // world->addLight(light);

  // renderToSDL(camera, world);
>>>>>>> 0d43310519f8353236ee83e9f8acc7717a6e27ca

  return 0;
}
#endif
