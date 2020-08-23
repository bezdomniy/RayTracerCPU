
#include <iostream>
#include <emscripten.h>

#include "camera.h"
#include "objectLoader.h"
#include "renderer.h"
#include "world.h"

extern "C"
{
    void processScene(char *data, int size)
    {
        std::shared_ptr<World> world;
        std::shared_ptr<Camera> camera;
        ObjectLoader objectLoader;

        std::string sceneDesc(data, size);
        std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);

        std::stringstream ss;

        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(camera, world);
        }
        std::string scene = ss.str();

        std::vector<char> byteBuffer(scene.begin(), scene.end());

        // add width as 2 bytes to returned array
        uint16_t width = camera->hsize;
        char widthLo = width & 0xFF;
        char widthHi = width >> 8;

        uint16_t height = camera->vsize;
        char heightLo = height & 0xFF;
        char heightHi = height >> 8;

        byteBuffer.push_back(widthHi);
        byteBuffer.push_back(widthLo);

        byteBuffer.push_back(heightHi);
        byteBuffer.push_back(heightLo);

        emscripten_worker_respond(&byteBuffer[0], byteBuffer.size());
    }

    void renderScene(char *data, int size)
    {
        std::string processedScene(data, size - 2);
        std::istringstream iss(processedScene);

        uint8_t *workerId = reinterpret_cast<uint8_t *>(data + size - 2);
        uint8_t *nWorkers = reinterpret_cast<uint8_t *>(data + size - 1);

        std::vector<std::pair<int, int>> pixelsToRender;

        std::shared_ptr<World> world;
        std::shared_ptr<Camera> camera;

        cereal::BinaryInputArchive iarchive(iss);
        iarchive(camera, world);

        Renderer renderer(camera);

        int sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
        double halfSubPixelSize = 1.0 / (double)(sqrtRaysPerPixel) / 2.0;

        pixelsToRender.clear();
        pixelsToRender.reserve((camera->vsize * camera->hsize) / (*nWorkers + 1));
        // TODO change to iterate over pixelstorender array
        for (int y = 0; y < camera->vsize; y++)
        {
            if (y % *nWorkers == *workerId)
            {
                for (int x = 0; x < camera->hsize; x++)
                {

                    // if (pixelsToRender.at((y * camera->hsize) + x) != '0')
                    // {
                    pixelsToRender.push_back(std::make_pair(x, y));
                    // }
                }
            }
        }

        std::vector<char> byteBuffer;
        size_t bufferLength;

        for (auto &pixel : pixelsToRender)
        {
            renderer.renderPixel(*world,
                                 pixel,
                                 sqrtRaysPerPixel,
                                 halfSubPixelSize);
        }

        std::tie(byteBuffer, bufferLength) = renderer.canvas.writeToRGBA(false);

        emscripten_worker_respond(&byteBuffer[0], bufferLength);
    }
}