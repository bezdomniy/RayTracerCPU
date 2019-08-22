#include "canvas.h"

Canvas::Canvas(unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    pixels.reserve(width * height);
}

Canvas::~Canvas()
{
}

void Canvas::writePixel(unsigned int x, unsigned int y, glm::vec3 colour) {
    pixels.at(y * width + x) = colour;
}

glm::vec3 Canvas::getPixel(unsigned int x, unsigned int y) {
    return pixels.at(y * width + x);
}
