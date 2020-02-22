#ifdef __EMSCRIPTEN__

#include "emscriptenRunner.h"

using namespace emscripten;

EmscriptenRunner::EmscriptenRunner() {
  this->sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
  this->halfSubPixelSize = 1.0 / (double)(this->sqrtRaysPerPixel) / 2.0;

  this->g = std::mt19937(this->rd());
}

EmscriptenRunner::~EmscriptenRunner() {}

void EmscriptenRunner::init(const std::string &sceneDesc) {
  this->camera = this->world.loadFromFile(sceneDesc);

  this->renderer = Renderer(this->camera);
  // this->renderer.canvas.clear(glm::dvec3(0.0, 0.0, 0.0));

  this->pixelsToRender.clear();

  this->pixelsToRender.reserve(this->camera->vsize * this->camera->hsize);

  for (int y = 0; y < this->camera->vsize; y++) {
    for (int x = 0; x < this->camera->hsize; x++) {
      this->pixelsToRender.push_back(std::make_pair(x, y));
    }
  }

  std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(),
               this->g);
}

void EmscriptenRunner::moveCamera(double posChange) {
  this->pixelsToRender.clear();
  // this->renderer.canvas.clear(glm::dvec3(0.0, 0.0, 0.0));

  for (int y = 0; y < this->camera->vsize; y++) {
    for (int x = 0; x < this->camera->hsize; x++) {
      this->pixelsToRender.push_back(std::make_pair(x, y));
    }
  }
  std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(),
               this->g);

  // glm::dmat4 rotationX =
  //     glm::rotate(glm::dmat4(1.0), posChange,
  //                 glm::dvec3(1.0, 0.0, 0.0));

  glm::dmat4 rotationY =
      glm::rotate(glm::dmat4(1.0), posChange, glm::dvec3(0.0, 1.0, 0.0));

  // glm::dmat4 rotationZ =
  //     glm::rotate(glm::dmat4(1.0), posChange,
  //                 glm::dvec3(0.0, 0.0, 1.0));

  this->camera->position = rotationY * this->camera->position;

  // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

  // this->camera->position.x += posChange;
  this->camera->updateTransform();
}

// void EmscriptenRunner::render_()
// {
//   for (int i = 0; i < PIXELS_PER_BATCH; i++)
//   {
//     if (!done())
//     {
//       this->this.renderPixel(this->world, this->pixelsToRender.back(),
//                              this->sqrtRaysPerPixel, this->halfSubPixelSize);
//       this->pixelsToRender.pop_back();
//     }
//   }
// }

emscripten::val EmscriptenRunner::renderToRGBA() {
  std::vector<uint8_t> byteBuffer;
  size_t bufferLength;

  for (int i = 0; i < PIXELS_PER_BATCH; i++) {
    if (!done()) {
      this->renderer.renderPixel(this->world, this->pixelsToRender.back(),
                                 this->sqrtRaysPerPixel,
                                 this->halfSubPixelSize);
      this->pixelsToRender.pop_back();
    }
  }

  // TODO parallelise this using taskflow - try just indexing instead of popping

  // tf::Executor executor(std::thread::hardware_concurrency());
  // tf::Taskflow taskflow;

  // for (int i = 0; i < PIXELS_PER_BATCH; i++) {
  //   taskflow.emplace([this]() { this.render_(); });
  // }

  // taskflow.parallel_for(this->pixelsToRender.begin(),
  //                       this->pixelsToRender.end(),
  //                       [this](auto &pixel) { this.render_(); });
  // executor.run(taskflow);
  // executor.wait_for_all();

  std::tie(byteBuffer, bufferLength) = this->renderer.canvas.writeToRGBA(false);

  return emscripten::val(
      emscripten::typed_memory_view(bufferLength, &byteBuffer[0]));
}

void EmscriptenRunner::moveLeft() { moveCamera(STEP_SIZE); }

void EmscriptenRunner::moveRight() { moveCamera(-STEP_SIZE); }

bool EmscriptenRunner::done() { return this->pixelsToRender.empty(); }

int EmscriptenRunner::getHeight() { return this->camera->vsize; }
int EmscriptenRunner::getWidth() { return this->camera->hsize; }

#endif