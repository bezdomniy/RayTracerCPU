#include "camera.h"
#include "renderer.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#endif

void run(const std::string &sceneDesc) {
  World world;
  std::shared_ptr<Camera> camera = world.loadFromFile(sceneDesc);

  Renderer renderer(camera);
  renderer.render(world);
  renderer.canvas.writeToPPM("out.ppm", false);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Module) { emscripten::function("runRayTracer", &run); }
int main(int argc, char const *argv[]) { return 0; }
#else
int main(int argc, char const *argv[]) {
  run("./scene2.yaml");
  return 0;
}
#endif
