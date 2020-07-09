#ifdef __EMSCRIPTEN__

#include <algorithm>
#include <memory>
// #include <random>
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

class EmscriptenRunner
{
private:
  // static const int PIXELS_PER_BATCH = 40000;
  //   unsigned int startCurrentBatch = 0;
  World world;
  std::shared_ptr<Camera> camera;
  Renderer renderer;
  std::vector<std::pair<int, int>> pixelsToRender;

  // std::vector<std::pair<int, int>>::iterator currentBatchEnd;

  int sqrtRaysPerPixel;
  double halfSubPixelSize;
  const double STEP_SIZE = 0.05f;

  // std::random_device rd;
  // std::mt19937 g;

  // void render_(EmscriptenRunner *runnerPtr);

  void moveCamera(double posChange);

public:
  EmscriptenRunner(/* args */);
  // void init(const std::string &sceneDesc, const std::string &pixelsToRender);
  ~EmscriptenRunner();

  emscripten::val renderToRGBA(const std::string &sceneDesc, const std::string &pixelsToRender);
  int getHeight();
  int getWidth();
  void moveLeft();
  void moveRight();
  // bool done();
};

#endif