
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
        cereal::BinaryOutputArchive oarchive(ss);
        oarchive(camera, world);
        std::string scene = ss.str();
        ss.seekg(0, std::ios::end);
        int ss_size = ss.tellg();
        ss.seekg(0, std::ios::beg);

        char *cstr = const_cast<char *>(scene.c_str());

        emscripten_worker_respond(cstr, ss_size);
    }

    void renderScene(char *data, int size)
    {
        std::string processedScene(data, size);
        std::istringstream iss(processedScene);

        std::cout << "size: " << size << std::endl;
        std::cout << "len : " << processedScene.size() << std::endl;

        std::vector<std::pair<int, int>> pixelsToRender;

        std::shared_ptr<World> world;
        std::shared_ptr<Camera> camera;

        cereal::BinaryInputArchive iarchive(iss);
        iarchive(camera, world);

        Renderer renderer(camera);

        int sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
        double halfSubPixelSize = 1.0 / (double)(sqrtRaysPerPixel) / 2.0;

        pixelsToRender.clear();
        pixelsToRender.reserve(camera->vsize * camera->hsize);

        // TODO change to iterate over pixelstorender array
        for (int y = 0; y < camera->vsize; y++)
        {
            for (int x = 0; x < camera->hsize; x++)
            {
                std::cout << x << " " << y << std::endl;
                // if (pixelsToRender.at((y * camera->hsize) + x) != '0')
                // {
                pixelsToRender.push_back(std::make_pair(x, y));
                // }
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