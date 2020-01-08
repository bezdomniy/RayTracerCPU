#include "camera.h"
#include "renderer.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
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

emscripten::val renderToRGBA(const std::string &sceneDesc)
{
  World world;
  std::shared_ptr<Camera> camera = world.loadFromFile(sceneDesc);

  Renderer renderer(camera);
  renderer.render(world);
  uint8_t *byteBuffer;
  size_t bufferLength;

  std::tie(byteBuffer,bufferLength) = renderer.canvas.writeToRGBA(false);

  return emscripten::val(emscripten::typed_memory_view(bufferLength, byteBuffer));
}

EMSCRIPTEN_BINDINGS(Module)
{
  emscripten::function("runRayTracerPPM", &renderToPPM);
  emscripten::function("runRayTracerRGBA", &renderToRGBA, emscripten::allow_raw_pointers());
}
int main(int argc, char const *argv[]) { return 0; }
#else
int main(int argc, char const *argv[])
{
  renderToPPM("scenes/hippy.yaml");

  // uint8_t *pixels = renderToRGBA("scenes/scene2.yaml");
  // for (int i = 0; i < 50 * 25 * 4; i += 4)
  // {
  //   std::cout << (int)pixels[i] << " " << (int)pixels[i + 1] << " " << (int)pixels[i + 2] << " " << (int)pixels[i + 3] << "\n";
  // }
  // delete pixels;

  return 0;
}
#endif
