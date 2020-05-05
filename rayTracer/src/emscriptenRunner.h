#ifdef __EMSCRIPTEN__

#include <algorithm>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include <emscripten.h>
#include <emscripten/val.h>

#include "camera.h"
#include "objectLoader.h"
#include "renderer.h"
#include "world.h"

class EmscriptenRunner {
private:
  static const int PIXELS_PER_BATCH = 20000;
  World world;
  std::shared_ptr<Camera> camera;
  Renderer renderer;
  std::vector<std::pair<int, int>> pixelsToRender;

  int sqrtRaysPerPixel;
  double halfSubPixelSize;
  const double STEP_SIZE = 0.05f;

  std::random_device rd;
  std::mt19937 g;

  void moveCamera(double posChange);

public:
  EmscriptenRunner(/* args */);
  void init(const std::string &sceneDesc);
  ~EmscriptenRunner();

  emscripten::val renderToRGBA();
  int getHeight();
  int getWidth();
  void moveLeft();
  void moveRight();
  bool done();
};

#endif