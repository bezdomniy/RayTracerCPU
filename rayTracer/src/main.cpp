#include "canvas.h"

int main(int argc, char const *argv[])
{
    Canvas c(10, 2);

    glm::vec3 c1(1.f, 0.8f, 0.6f);

    c.clear(c1);

    // glm::vec3 c1(1.5f, 0.f, 0.f);
    // glm::vec3 c2(0.f, 0.5f, 0.f);
    // glm::vec3 c3(-0.5f, 0.f, 1.f);

    // c.writePixel(0, 0, c1);
    // c.writePixel(2, 1, c2);
    // c.writePixel(4, 2, c3);


    c.writeToPPM("/home/ilia/dev/gamedev/sdl_game/rayTracer/tests/test.ppm");
    return 0;
}
