// #ifdef __EMSCRIPTEN__

// #include "emscriptenRunner.h"

// using namespace emscripten;

// EmscriptenRunner::EmscriptenRunner()
// {
//     this->sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
//     this->halfSubPixelSize = 1.0 / (double)(this->sqrtRaysPerPixel) / 2.0;

//     this->g = std::mt19937(this->rd());
// }

// EmscriptenRunner::~EmscriptenRunner() {}

// void EmscriptenRunner::moveCamera(double posChange)
// {
//     this->pixelsToRender.clear();
//     // this->renderer.canvas.clear(glm::dvec3(0.0, 0.0, 0.0));

//     // for (int y = 0; y < this->camera->vsize; y++)
//     // {
//     //   for (int x = 0; x < this->camera->hsize; x++)
//     //   {
//     //     this->pixelsToRender.push_back(std::make_pair(x, y));
//     //   }
//     // }
//     // std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(),
//     //              this->g);

//     // this->currentBatchEnd = this->pixelsToRender.begin() + PIXELS_PER_BATCH;

//     // glm::dmat4 rotationX =
//     //     glm::rotate(glm::dmat4(1.0), posChange,
//     //                 glm::dvec3(1.0, 0.0, 0.0));

//     glm::dmat4 rotationY =
//         glm::rotate(glm::dmat4(1.0), posChange, glm::dvec3(0.0, 1.0, 0.0));

//     // glm::dmat4 rotationZ =
//     //     glm::rotate(glm::dmat4(1.0), posChange,
//     //                 glm::dvec3(0.0, 0.0, 1.0));

//     this->camera->position = rotationY * this->camera->position;

//     // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

//     // this->camera->position.x += posChange;
//     this->camera->updateTransform();
// }

// emscripten::val EmscriptenRunner::renderToRGBA(const std::string &pixelsToRender)
// {
//     // TODO change to iterate over pixelstorender array
//     for (int y = 0; y < this->camera->vsize; y++)
//     {
//         for (int x = 0; x < this->camera->hsize; x++)
//         {
//             // std::cout << pixelsToRender.at((y * this->camera->hsize) + x) << std::endl;
//             if (pixelsToRender.at((y * this->camera->hsize) + x) != '0')
//             {
//                 this->pixelsToRender.push_back(std::make_pair(x, y));
//             }
//             // this->pixelsToRender.push_back(std::make_pair(x, y));
//         }
//     }

//     std::vector<uint8_t> byteBuffer;
//     size_t bufferLength;

//     for (auto &pixel : this->pixelsToRender)
//     {

//         this->renderer.renderPixel(*this->world,
//                                    pixel,
//                                    this->sqrtRaysPerPixel,
//                                    this->halfSubPixelSize);
//         // this->pixelsToRender.pop_back();
//     }

//     std::tie(byteBuffer, bufferLength) = this->renderer.canvas.writeToRGBA(false);

//     return emscripten::val(
//         emscripten::typed_memory_view(bufferLength, &byteBuffer[0]));
// }

// // emscripten::val EmscriptenRunner::renderToRGBAThreaded()
// // {
// //   std::vector<uint8_t> byteBuffer;
// //   size_t bufferLength;

// //   tf::Executor executor(std::thread::hardware_concurrency());
// //   tf::Taskflow taskflow;

// //   std::vector<std::pair<int, int>>::reverse_iterator endBatch;
// //   size_t remaining =
// //       std::distance(this->pixelsToRender.rbegin(), this->pixelsToRender.rend());

// //   if (remaining > PIXELS_PER_BATCH)
// //   {
// //     endBatch = this->pixelsToRender.rbegin() + PIXELS_PER_BATCH;
// //   }
// //   else
// //   {
// //     endBatch = this->pixelsToRender.rend();
// //   }

// //   std::cout << "start: " << remaining << std::endl;

// //   // std::cout << "s: " << remaining << "e: " << endBatch << std::endl;

// //   taskflow.parallel_for(
// //       this->pixelsToRender.rbegin(), endBatch, [this](auto &pixel) {
// //         this->renderer.renderPixel(*this->world, pixel, this->sqrtRaysPerPixel,
// //                                    this->halfSubPixelSize);
// //       });
// //   executor.run(taskflow);
// //   executor.wait_for_all();

// //   std::cout << "done: " << remaining << std::endl;

// //   for (int i = 0; i < PIXELS_PER_BATCH; i++)
// //   {
// //     if (!done())
// //     {
// //       this->pixelsToRender.pop_back();
// //     }
// //   }

// //   std::tie(byteBuffer, bufferLength) = this->renderer.canvas.writeToRGBA(false);

// //   return emscripten::val(
// //       emscripten::typed_memory_view(bufferLength, &byteBuffer[0]));
// // }

// emscripten::val EmscriptenRunner::renderProcessedToRGBA(const std::string &processedScene, const std::string &pixelsToRender)
// {
//     std::istringstream iss(processedScene);

//     // iss.seekg(0, std::ios::end);
//     // int size = iss.tellg();
//     // iss.seekg(0, std::ios::beg);

//     // std::cout << "Loaded size: " << size << std::endl;

//     cereal::BinaryInputArchive iarchive(iss);
//     iarchive(this->camera, this->world);

//     this->renderer = Renderer(this->camera);

//     this->pixelsToRender.clear();

//     this->pixelsToRender.reserve(pixelsToRender.length());

//     // TODO change to iterate over pixelstorender array
//     for (int y = 0; y < this->camera->vsize; y++)
//     {
//         for (int x = 0; x < this->camera->hsize; x++)
//         {
//             if (pixelsToRender.at((y * this->camera->hsize) + x) != '0')
//             {
//                 this->pixelsToRender.push_back(std::make_pair(x, y));
//             }
//         }
//     }

//     std::vector<uint8_t> byteBuffer;
//     size_t bufferLength;

//     for (auto &pixel : this->pixelsToRender)
//     {

//         this->renderer.renderPixel(*this->world,
//                                    pixel,
//                                    this->sqrtRaysPerPixel,
//                                    this->halfSubPixelSize);
//     }

//     std::tie(byteBuffer, bufferLength) = this->renderer.canvas.writeToRGBA(false);

//     return emscripten::val(
//         emscripten::typed_memory_view(bufferLength, &byteBuffer[0]));
// }

// emscripten::val EmscriptenRunner::processScene(const std::string &sceneDesc, bool returnArchive)
// {
//     ObjectLoader objectLoader;
//     std::tie(this->camera, this->world) = objectLoader.loadYaml(sceneDesc);

//     this->renderer = Renderer(this->camera);
//     this->pixelsToRender.clear();
//     this->pixelsToRender.reserve(this->camera->vsize * this->camera->hsize);

//     for (int y = 0; y < this->camera->vsize; y++)
//     {
//         for (int x = 0; x < this->camera->hsize; x++)
//         {
//             this->pixelsToRender.push_back(std::make_pair(x, y));
//         }
//     }

//     std::shuffle(this->pixelsToRender.begin(), this->pixelsToRender.end(),
//                  this->g);

//     if (returnArchive)
//     {
//         std::stringstream ss;
//         cereal::BinaryOutputArchive oarchive(ss);
//         oarchive(this->camera, this->world);

//         // std::string out = ss.str();
//         // return emscripten::val(out);

//         // const std::string &tmp = ss.str();
//         this->scene = ss.str();
//         ss.seekg(0, std::ios::end);
//         int size = ss.tellg();
//         ss.seekg(0, std::ios::beg);

//         // std::cout << "Processed size: " << size << std::endl;
//         // size_t length = tmp.length();
//         const char *cstr = this->scene.c_str();

//         return emscripten::val(
//             emscripten::typed_memory_view(size, cstr));
//     }

//     return emscripten::val::null();
// }

// void EmscriptenRunner::moveLeft()
// {
//     moveCamera(STEP_SIZE);
// }

// void EmscriptenRunner::moveRight() { moveCamera(-STEP_SIZE); }

// bool EmscriptenRunner::done() { return this->pixelsToRender.empty(); }

// int EmscriptenRunner::getHeight() { return this->camera->vsize; }
// int EmscriptenRunner::getWidth() { return this->camera->hsize; }

// #endif