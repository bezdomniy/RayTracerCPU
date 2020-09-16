#include "window.h"
#include <fstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
// #include <iostream>

Window window;

// TODO .obj files should only be downloaded by the processing worker, not the render workers
// TODO texture files can be set to the url path on iliathoughts, rather than being put into the .data file
// TODO maybe even the obj files can just be downloaded ad-hoc from the server
void processCback(char *data, int size, void *arg)
{
  window.sceneBinary = std::vector<char>(data, data + size - (sizeof(int) * 2));

  std::cout << "Scene binary size: " << window.sceneBinary.size() << std::endl;

  window.width = *reinterpret_cast<int *>(data + size - (sizeof(int) * 2));
  window.height = *reinterpret_cast<int *>(data + size - sizeof(int));

  if (!window.initialised)
    window.initWindow();
  else
    window.updateSize();

  // window.destroyProcessorWorker();
  window.somethingChanged = true;

  // window.killWorker();
  // window.addWorker();

  // window.update();
}

void renderCback(char *data, int size, void *arg)
{
  // std::cout << "Render Callback: " << size << std::endl;

  // window.pixelsBinary = std::vector<char>(data, data + size - 1);
  uint8_t *workerId = reinterpret_cast<uint8_t *>(data + size - sizeof(uint8_t));

  window.pixelsBinary[*workerId] = std::vector<char>(data, data + size - sizeof(uint8_t));

  window.draw(*workerId);
  // window.somethingChanged = false;

  window.busyWorkers.at(*workerId) = false;

  if (std::all_of(window.busyWorkers.begin(), window.busyWorkers.end(), [](bool v) { return !v; }))
    window.running = false;
  // std::cout << size << std::endl;
}

void loop()
{
  window.step();
}

// int EMSCRIPTEN_KEEPALIVE main(int argc, char const *argv[])
// {
//   window.processScene("/scenes/christmas.yaml");

//   // window.step();
//   emscripten_set_main_loop(loop, 2, true);
// }

#ifdef __cplusplus
extern "C"
{
#endif
  void EMSCRIPTEN_KEEPALIVE killWorker()
  {
    window.killWorker();
  }

  void EMSCRIPTEN_KEEPALIVE draw(const char *s)
  {
    std::string sceneDesc(s);
    window.processScene(sceneDesc);
    window.xRotation = 0.0f;
    window.yRotation = 0.0f;

    // window.step();
    if (!window.initialised)
    {
      std::cout << "starting loop" << std::endl;
      emscripten_set_main_loop(loop, 0, true);
    }
  }

  void EMSCRIPTEN_KEEPALIVE moveLeft()
  {
    window.moveLeft();
  }

  void EMSCRIPTEN_KEEPALIVE moveRight()
  {
    window.moveRight();
  }

// void EMSCRIPTEN_KEEPALIVE getModel(const char *url)
// {
//   std::string urlText(url);

//   std::cout << "url: : " << urlText << '\n';

//   std::size_t found = urlText.find_last_of("/\\");
//   std::string newPath = "/models/" + urlText.substr(found + 1);

//   emscripten_wget(url, newPath.c_str());
//   std::cout << " loaded to: "
//             << newPath << std::endl;
// }
#ifdef __cplusplus
}
#endif

#else

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void renderToPPM(const std::string &sceneDesc)
{
  std::shared_ptr<World> world;
  std::shared_ptr<Camera> camera;

  ObjectLoader objectLoader;
  std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);

  Renderer renderer(camera);
  renderer.render(*world);
  renderer.canvas.writeToPPM("out.ppm", false);
}

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

#include <cmath>
std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> rayTracerInOneWeekendCover()
{
  std::vector<std::shared_ptr<Shape>> shapes;
  shapes.reserve((22 * 22) + 4);
  std::shared_ptr<Group> mesh;

  std::shared_ptr<Material> planeMaterial = std::make_shared<Material>();
  planeMaterial->reflective = 0.1;
  planeMaterial->colour = glm::dvec3(0.8, 0.8, 0.8);

  std::shared_ptr<Material> sphereMaterial = std::make_shared<Material>();
  sphereMaterial->reflective = 0.8;
  sphereMaterial->colour = glm::dvec3(0.8, 0.1, 0.1);

  std::shared_ptr<World> world = std::make_shared<World>();
  std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(13., 2., 3., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 384, 216, 0.524);

  // std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(13., 2., 3., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 800, 600, 0.524);

  std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
      glm::dvec4(glm::dvec4(3., 10., -3., 1.)),
      glm::dvec3(1., 1., 1.));

  std::shared_ptr<Shape> plane = std::make_shared<Plane>();
  plane->setMaterial(planeMaterial);

  world->addShape(plane);
  // shapes.push_back(plane);

  for (int a = -11; a < 11; a++)
  {
    for (int b = -11; b < 11; b++)
    {
      auto choose_mat = rand() / (RAND_MAX + 1.0);
      glm::dvec3 center(a + 0.9 * (rand() / (RAND_MAX + 1.0)), 0.2, b + 0.9 * (rand() / (RAND_MAX + 1.0)));

      if ((center - glm::dvec3(4., 0.2, 0.)).length() > 0.9)
      {
        std::shared_ptr<Shape> sphere = std::make_shared<Sphere>();
        sphere->setMaterial(sphereMaterial);

        glm::dmat4 scale =
            glm::scale(glm::dmat4(1.0), glm::dvec3(0.2, 0.2, 0.2));

        glm::dmat4 translate =
            glm::translate(glm::dmat4(1.0), center);

        sphere->multiplyTransform(scale);
        sphere->multiplyTransform(translate);
        sphere->calculateInverseTranform();

        // world->addShape(sphere);
        shapes.push_back(sphere);
      }
    }
  }

  std::shared_ptr<Material> sphereMaterialRefractive = std::make_shared<Material>();

  sphereMaterialRefractive->transparency = 0.5;
  sphereMaterialRefractive->refractiveIndex = 1.52;
  sphereMaterialRefractive->colour = glm::dvec3(0.1, 0.8, 0.1);

  std::shared_ptr<Material> sphereMaterialBase = std::make_shared<Material>();
  // sphereMaterial->reflective = 0.8;
  sphereMaterialBase->colour = glm::dvec3(0.1, 0.1, 0.8);

  std::shared_ptr<Shape> sphere1 = std::make_shared<Sphere>();
  sphere1->setMaterial(sphereMaterialRefractive);
  glm::dmat4 translate1 =
      glm::translate(glm::dmat4(1.0), glm::dvec3(0., 1., 0.));
  sphere1->multiplyTransform(translate1);
  sphere1->calculateInverseTranform();
  // world->addShape(sphere1);
  shapes.push_back(sphere1);

  std::shared_ptr<Shape> sphere2 = std::make_shared<Sphere>();
  sphere2->setMaterial(sphereMaterialBase);
  glm::dmat4 translate2 =
      glm::translate(glm::dmat4(1.0), glm::dvec3(-4., 1., 0.));
  sphere2->multiplyTransform(translate2);
  sphere2->calculateInverseTranform();
  // world->addShape(sphere2);
  shapes.push_back(sphere2);

  std::shared_ptr<Shape> sphere3 = std::make_shared<Sphere>();
  sphere3->setMaterial(sphereMaterial);
  glm::dmat4 translate3 =
      glm::translate(glm::dmat4(1.0), glm::dvec3(4., 1., 0.));
  sphere3->multiplyTransform(translate3);
  sphere3->calculateInverseTranform();
  // world->addShape(sphere3);
  shapes.push_back(sphere3);

  mesh = Model::buildBoundingVolumeHierarchy(shapes);

  // TODO: this is a hack - top level group in bvh should not need material, but does otherwise you get seg fault. fix this
  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.8, 0.8, 0.8);
  mesh->setMaterial(material);

  std::shared_ptr<Shape> meshShape = std::dynamic_pointer_cast<Shape>(mesh);
  world->addShape(meshShape);

  world->addLight(light);

  // Renderer renderer(camera);
  // renderer.render(*world);
  // renderer.canvas.writeToPPM("rtiow.ppm", false);

  return std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>>(camera, world);
}

Model mesh(const std::string &objPath)
{
  Model model(objPath, true);
  std::shared_ptr<Material> material = std::make_shared<Material>();
  material->colour = glm::dvec3(0.8, 0.8, 0.8);
  model.mesh->setMaterial(material);

  glm::dmat4 scale =
      glm::scale(glm::dmat4(1.0), glm::dvec3(0.04, 0.04, 0.04));
  // glm::dmat4 rotationy =
  //     glm::rotate(glm::dmat4(1.0), 3.8,
  //                 glm::dvec3(0.0, 1.0, 0.0));
  glm::dmat4 translate =
      glm::translate(glm::dmat4(1.0), glm::dvec3(0., -3, 0.));

  model.mesh->multiplyTransform(scale);
  // model.mesh->multiplyTransform(rotationy);
  model.mesh->multiplyTransform(translate);
  model.mesh->calculateInverseTranform();

  return model;
}

std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>> draw3Dmodel(std::string modelFile)
{
  std::shared_ptr<World> world = std::make_shared<World>();
  std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::dvec4(5., 5, -20., 1.), glm::dvec4(0., 0., 0., 1.), glm::dvec4(0., 1., 0., 0.), 400, 400, 0.524);

  Model model = mesh(modelFile);
  // Model model2 = model;
  // Model model3 = model;
  // Model model4 = model;
  // Model model5 = model;

  std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
      glm::dvec4(glm::dvec4(10., 10., -10., 1.)),
      glm::dvec3(1., 1., 1.));

  std::shared_ptr<Shape> mesh = std::dynamic_pointer_cast<Shape>(model.mesh);
  // std::shared_ptr<Shape> mesh2 = std::dynamic_pointer_cast<Shape>(model2.mesh);
  // std::shared_ptr<Shape> mesh3 = std::dynamic_pointer_cast<Shape>(model3.mesh);
  // std::shared_ptr<Shape> mesh4 = std::dynamic_pointer_cast<Shape>(model4.mesh);
  // std::shared_ptr<Shape> mesh5 = std::dynamic_pointer_cast<Shape>(model5.mesh);

  // glm::dmat4 translateMesh =
  //     glm::translate(glm::dmat4(1.0), glm::dvec3(0., 2, 0.));
  // mesh2->multiplyTransform(translateMesh);
  // mesh2->calculateInverseTranform();

  // translateMesh =
  //     glm::translate(glm::dmat4(1.0), glm::dvec3(0., -2, 0.));
  // mesh3->multiplyTransform(translateMesh);
  // mesh3->calculateInverseTranform();

  // translateMesh =
  //     glm::translate(glm::dmat4(1.0), glm::dvec3(2., 2, 0.));
  // mesh4->multiplyTransform(translateMesh);
  // mesh4->calculateInverseTranform();

  // translateMesh =
  //     glm::translate(glm::dmat4(1.0), glm::dvec3(2., -2, 0.));
  // mesh5->multiplyTransform(translateMesh);
  // mesh5->calculateInverseTranform();

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
  // world->addShape(mesh2);
  // world->addShape(mesh3);
  // world->addShape(mesh4);
  // world->addShape(mesh5);
  // world->addShape(plane);
  world->addLight(light);

  return std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>>(camera, world);
}

void run(void *arg)
{
  Window *window = static_cast<Window *>(arg);
  window->step();
}

// bool fileExists(const std::string &name)
// {
//   ifstream f(name.c_str());
//   return f.good();
// }

// #include <filesystem>

int main(int argc, char const *argv[])
{
  // rayTracerInOneWeekendCover();

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

  // renderToSDL("assets/scenes/reflectionScene.yaml");
  // renderToSDL("assets/scenes/coverScene.yaml");
  // renderToSDL("assets/scenes/groups.yaml");
  renderToPPM("assets/scenes/model.yaml");
  // renderToSDL("assets/scenes/hippy.yaml");
  // renderToSDL("assets/scenes/globe.yaml");
  // renderToSDL("assets/scenes/skybox.yaml");
  // renderToSDL("assets/scenes/checkers.yaml");
  // renderToSDL("assets/scenes/christmas.yaml");
  // renderToSDL("assets/scenes/reflectionScene.yaml");
  // renderToSDL("assets/scenes/reflectionScene.yaml");

  // renderToPPM("scenes/christmas.yaml");
  // renderToPPM("scenes/reflectionScene.yaml");

  // std::shared_ptr<World> world;
  // std::shared_ptr<Camera> camera;

  // std::tie(camera, world) = draw3Dmodel("assets/models/armadillo.obj");

  // Renderer renderer(camera);
  // renderer.render(*world);
  // renderer.canvas.writeToPPM("armadillo.ppm", false);

  // renderToSDL(camera, world);

  return 0;
}

#endif //EMSCRIPTEN