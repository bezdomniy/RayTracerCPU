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
  window.sceneBinary = std::vector<char>(data, data + size);

  std::cout << "Scene binary size: " << window.sceneBinary.size() << std::endl;

  window.width = *reinterpret_cast<int *>(data + size - window.offset);
  window.height = *reinterpret_cast<int *>(data + size - window.offset + sizeof(int));

  if (!window.initialised)
    window.initWindow();
  else
    window.updateSize();

  window.somethingChanged = true;
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
}

void loop()
{
  window.step();
}

#ifdef __cplusplus
extern "C"
{
#endif
  void EMSCRIPTEN_KEEPALIVE killWorker()
  {
    window.killWorker();
  }

  void EMSCRIPTEN_KEEPALIVE draw(const char *s, int c)
  {
    std::string sceneDesc(s);
    window.nWorkers = c;
    std::cout << "nworkers: " << window.nWorkers << std::endl;
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

#ifdef __cplusplus
}
#endif

#else

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>

void renderToPPM(const std::string &sceneDesc)
{
  std::shared_ptr<World> world;
  std::shared_ptr<Camera> camera;

  ObjectLoader objectLoader;
  std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);

  Renderer renderer(camera);
  auto start = std::chrono::high_resolution_clock::now();
  renderer.render(*world);
  auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "Time to render image: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

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

  mesh = std::make_shared<Group>(shapes, true);

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

void run(void *arg)
{
  Window *window = static_cast<Window *>(arg);
  window->step();
}

// bool fileExists(const char *name)
// {
//   std::ifstream f(name);
//   return f.good();
// }

#include <filesystem>

int main(int argc, char const *argv[])
{
  std::string option(argv[1]);
  std::string f(argv[2]);

  if (!std::filesystem::exists(f))
  {
    std::cout << "File does not exist: " << std::filesystem::absolute(f) << std::endl;
    exit(0);
  }

  if (option == "-p")
    renderToPPM(f);
  else if (option == "-s")
    renderToSDL(f);
  else
    std::cout << "Invalid option: " << option << std::endl;

  return 0;
}

#endif //EMSCRIPTEN