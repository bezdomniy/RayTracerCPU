#pragma once

#include <vector>
#include <glm/glm.hpp>

class Canvas
{
private:
    unsigned int width;
    unsigned int height;
    std::vector<glm::vec3> pixels;
public:
    Canvas(unsigned int, unsigned int);
    ~Canvas();

    void writePixel(unsigned int, unsigned int, glm::vec3);
    glm::vec3 getPixel(unsigned int, unsigned int);
};

