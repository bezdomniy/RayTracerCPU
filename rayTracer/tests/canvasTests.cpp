#include "../src/canvas.h"
#include "testUtils.h"
#include <iostream>

bool testWritePixel() {
    Canvas c(10, 20);
    glm::vec3 red(1.f,0.f,0.f);

    c.writePixel(2, 3, red);

    return c.getPixel(2, 3) == red;
}

bool testWritePPM() {
    Canvas c(10, 20);

    glm::vec3 red(1.f,0.f,0.f);

    c.writePixel(2, 3, red);
    c.writePixel(3, 3, red);
    c.writePixel(4, 3, red);
    c.writePixel(2, 4, red);
    c.writePixel(3, 4, red);
    c.writePixel(4, 4, red);

    c.writeToPPM("/home/ilia/dev/gamedev/sdl_game/rayTracer/tests/test.ppm");

    return true;
}

int main(int argc, char const *argv[])
{
    ASSERT_THROW(testWritePixel());
    testWritePPM();
    return 0;
}