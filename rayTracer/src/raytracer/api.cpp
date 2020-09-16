
#include <iostream>
#include <memory>

#include "camera.h"
#include "objectLoader.h"
#include "renderer.h"
#include "world.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#include <streambuf>
struct membuf : std::streambuf
{
    membuf(char *begin, char *end)
    {
        this->setg(begin, begin, end);
    }
};

std::vector<char> __processScene(const std::string &sceneDesc)
{
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;
    std::string scene;
    ObjectLoader objectLoader;
    std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);
    std::stringstream ss;

    {
        cereal::BinaryOutputArchive oarchive(ss);
        oarchive(camera, world);
    }
    scene = ss.str();

    std::vector<char> byteBuffer = std::vector<char>(scene.begin(), scene.end());
    // }

    uint8_t *widthBytePointer = reinterpret_cast<uint8_t *>(&(camera->hsize));
    std::vector<uint8_t> widthBytes(widthBytePointer, widthBytePointer + sizeof(int));
    byteBuffer.insert(byteBuffer.end(), widthBytes.begin(), widthBytes.end());

    uint8_t *heightBytePointer = reinterpret_cast<uint8_t *>(&(camera->vsize));
    std::vector<uint8_t> heightBytes(heightBytePointer, heightBytePointer + sizeof(int));
    byteBuffer.insert(byteBuffer.end(), heightBytes.begin(), heightBytes.end());
    // }
    return byteBuffer;
}

extern "C"
{
    void processScene(char *data, int size)
    {

        // ObjectLoader objectLoader;
        const std::string sceneDesc(data, size);

        // std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);
        // std::stringstream ss;

        // {
        //     cereal::BinaryOutputArchive oarchive(ss);
        //     oarchive(camera, world);
        // }
        // scene = ss.str();

        // std::vector<char> byteBuffer = std::vector<char>(scene.begin(), scene.end());

        // uint8_t *widthBytePointer = reinterpret_cast<uint8_t *>(&(camera->hsize));
        // std::vector<uint8_t> widthBytes(widthBytePointer, widthBytePointer + sizeof(int));
        // byteBuffer.insert(byteBuffer.end(), widthBytes.begin(), widthBytes.end());

        // uint8_t *heightBytePointer = reinterpret_cast<uint8_t *>(&(camera->vsize));
        // std::vector<uint8_t> heightBytes(heightBytePointer, heightBytePointer + sizeof(int));
        // byteBuffer.insert(byteBuffer.end(), heightBytes.begin(), heightBytes.end());

        std::vector<char> byteBuffer = __processScene(sceneDesc);

        emscripten_worker_respond(&byteBuffer[0], byteBuffer.size());
    }

    void renderScene(char *data, int size)
    {
        uint8_t *workerId = reinterpret_cast<uint8_t *>(data + size - 2);
        uint8_t *nWorkers = reinterpret_cast<uint8_t *>(data + size - 1);

        float *xRotation = reinterpret_cast<float *>(data + size - 10);
        float *yRotation = reinterpret_cast<float *>(data + size - 6);

        // std::cout << "Rotations: " << *xRotation << " " << *yRotation << std::endl;

        // std::string processedScene(data, size - 10);
        // std::istringstream iss(processedScene);

        // std::istringstream iss;
        // iss.rdbuf()->pubsetbuf(data, size - 10);
        membuf sbuf(data, data + size - 10);
        std::istream iss(&sbuf);

        // std::vector<std::pair<int, int>> pixelsToRender;

        std::shared_ptr<World> world;
        std::shared_ptr<Camera> camera;

        cereal::BinaryInputArchive iarchive(iss);
        iarchive(camera, world);

        glm::dmat4 rotationX =
            glm::rotate(glm::dmat4(1.0), (double)*xRotation, glm::dvec3(1.0, 0.0, 0.0));

        glm::dmat4 rotationY =
            glm::rotate(glm::dmat4(1.0), (double)*yRotation, glm::dvec3(0.0, 1.0, 0.0));

        // glm::dmat4 rotationZ =
        //     glm::rotate(glm::dmat4(1.0), posChange,
        //                 glm::dvec3(0.0, 0.0, 1.0));

        camera->position = rotationX * camera->position;
        camera->position = rotationY * camera->position;

        // matrix.makeRotationY(clock.getDelta() * 2 * Math.PI / period);

        // this->camera->position.x += posChange;
        camera->updateTransform();

        Renderer renderer(camera);

        int sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
        double halfSubPixelSize = 1.0 / (double)(sqrtRaysPerPixel) / 2.0;

        // pixelsToRender.clear();
        // pixelsToRender.reserve((camera->vsize * camera->hsize) / (*nWorkers + 1));

        // TODO change to iterate over pixelstorender array
        for (int y = 0; y < camera->vsize; y++)
        {
            if (y % *nWorkers == *workerId)
            {
                for (int x = 0; x < camera->hsize; x++)
                {
                    // pixelsToRender.push_back(std::make_pair(x, y));

                    // TODO could just get this to write string to bytebuffer to save mem
                    // TODO check if rendering just 1 pixel will give same memory outcome as rendering all of them
                    auto pixel = std::make_pair(x, y);
                    renderer.renderPixel(*world,
                                         pixel,
                                         sqrtRaysPerPixel,
                                         halfSubPixelSize);
                }
            }
        }

        std::vector<char> byteBuffer;
        size_t bufferLength;

        // for (auto &pixel : pixelsToRender)
        // {
        //     renderer.renderPixel(*world,
        //                          pixel,
        //                          sqrtRaysPerPixel,
        //                          halfSubPixelSize);
        // }

        std::tie(byteBuffer, bufferLength) = renderer.canvas.writeToRGBA(false);

        byteBuffer.push_back(*(data + size - 2));

        emscripten_worker_respond(&byteBuffer[0], bufferLength + 1);
    }

    void renderSceneThreaded(char *data, int size)
    {
        uint8_t *workerId = reinterpret_cast<uint8_t *>(data + size - 2);
        uint8_t *nWorkers = reinterpret_cast<uint8_t *>(data + size - 1);

        float *xRotation = reinterpret_cast<float *>(data + size - 10);
        float *yRotation = reinterpret_cast<float *>(data + size - 6);
        membuf sbuf(data, data + size - 10);
        std::istream iss(&sbuf);

        std::shared_ptr<World> world;
        std::shared_ptr<Camera> camera;

        cereal::BinaryInputArchive iarchive(iss);
        iarchive(camera, world);

        glm::dmat4 rotationX =
            glm::rotate(glm::dmat4(1.0), (double)*xRotation, glm::dvec3(1.0, 0.0, 0.0));

        glm::dmat4 rotationY =
            glm::rotate(glm::dmat4(1.0), (double)*yRotation, glm::dvec3(0.0, 1.0, 0.0));

        // glm::dmat4 rotationZ =
        //     glm::rotate(glm::dmat4(1.0), posChange,
        //                 glm::dvec3(0.0, 0.0, 1.0));

        camera->position = rotationX * camera->position;
        camera->position = rotationY * camera->position;

        camera->updateTransform();

        Renderer renderer(camera);

        // int sqrtRaysPerPixel = (int)std::sqrt(Renderer::RAYS_PER_PIXEL);
        // double halfSubPixelSize = 1.0 / (double)(sqrtRaysPerPixel) / 2.0;

        renderer.render(*world);

        std::vector<char> byteBuffer;
        size_t bufferLength;
        std::tie(byteBuffer, bufferLength) = renderer.canvas.writeToRGBA(false);

        byteBuffer.push_back(*(data + size - 2));

        emscripten_worker_respond(&byteBuffer[0], bufferLength + 1);
    }
}
// #else

// std::vector<glm::dvec3> renderScene(const std::string &sceneDesc)
// {
//     std::shared_ptr<World> world;
//     std::shared_ptr<Camera> camera;

//     ObjectLoader objectLoader;
//     std::tie(camera, world) = objectLoader.loadYaml(sceneDesc);
//     Renderer rayTraceRenderer = Renderer(camera);
//     rayTraceRenderer.render(*world);

//     return rayTraceRenderer.canvas.pixels;
// }
#endif