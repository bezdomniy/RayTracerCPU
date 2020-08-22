// #ifdef __EMSCRIPTEN__

// #include <algorithm>
// #include <memory>
// #include <random>
// #include <string>
// #include <utility>
// #include <vector>

// #include <glm/glm.hpp>

// #include <emscripten.h>
// #include <emscripten/val.h>

// #include <cereal/archives/binary.hpp>
// #include <sstream>
// #include <iostream>

// #include "camera.h"
// #include "objectLoader.h"
// #include "renderer.h"
// #include "world.h"

// class EmscriptenRunner
// {
// private:
//   static const int PIXELS_PER_BATCH = 40000;
//   //   unsigned int startCurrentBatch = 0;
//   // World world;
//   std::shared_ptr<World> world;
//   std::shared_ptr<Camera> camera;
//   Renderer renderer;
//   std::vector<std::pair<int, int>> pixelsToRender;

//   // std::vector<std::pair<int, int>>::iterator currentBatchEnd;

//   int sqrtRaysPerPixel;
//   double halfSubPixelSize;
//   const double STEP_SIZE = 0.05f;

//   std::string scene;
//   std::random_device rd;
//   std::mt19937 g;

//   void moveCamera(double posChange);

// public:
//   EmscriptenRunner(/* args */);
//   ~EmscriptenRunner();

//   emscripten::val processScene(const std::string &sceneDesc, bool returnArchive = true);
//   emscripten::val renderToRGBA(const std::string &pixelsToRender);
//   // emscripten::val renderToRGBAThreaded();
//   emscripten::val renderProcessedToRGBA(const std::string &processedScene, const std::string &pixelsToRender);
//   int getHeight();
//   int getWidth();
//   void moveLeft();
//   void moveRight();
//   bool done();
// };

// #endif