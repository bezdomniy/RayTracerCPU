#ifdef __EMSCRIPTEN__

#include "emscriptenRunner.h"

using namespace emscripten;

EmscriptenRunner::EmscriptenRunner() {
  this->sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
  this->halfSubPixelSize = 1.f / (float)(this->sqrtRaysPerPixel) / 2.f;

  this->g = std::mt19937(this->rd());
}

EmscriptenRunner::~EmscriptenRunner() {}

void EmscriptenRunner::init(const std::string &sceneDesc) {
  this->camera = this->world.loadFromFile(sceneDesc);

  this->renderer = Renderer(this->camera);
  // this->renderer.canvas.clear(glm::vec3(0.f, 0.f, 0.f));

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

void EmscriptenRunner::moveCamera(float posChange) {
  this->pixelsToRender.clear();
  // this->renderer.canvas.clear(glm::vec3(0.f, 0.f, 0.f));

  for (int y = 0; y < this->camera->vsize; y++) {
    for (int x = 0; x < this->camera->hsize; x++) {
      this->pixelsToRender.push_back(std::make_pair(x, y));
    }
  }
  std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(),
               this->g);

  // glm::mat4 rotationX =
  //     glm::rotate(glm::mat4(1.f), posChange,
  //                 glm::vec3(1.f, 0.f, 0.f));

  glm::mat4 rotationY =
      glm::rotate(glm::mat4(1.f), posChange, glm::vec3(0.f, 1.f, 0.f));

  // glm::mat4 rotationZ =
  //     glm::rotate(glm::mat4(1.f), posChange,
  //                 glm::vec3(0.f, 0.f, 1.f));

  this->camera->position = rotationY * this->camera->position;

  // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

  // this->camera->position.x += posChange;
  this->camera->updateTransform();
}

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