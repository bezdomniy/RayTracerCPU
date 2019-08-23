#include "canvas.h"
#include "projectile.h"
#include "environment.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    Canvas c(900, 550);

    glm::vec3 c1(1.f, 0.f, 0.f);
    glm::vec3 c2(0.f, 0.f, 0.f);

    c.clear(c2);

    Projectile p(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.8f, 0.f), 11.25f);

    std::unique_ptr<Environment> environment = std::make_unique<Environment>(glm::vec3(0.f, -0.1f, 0.f), glm::vec3(-0.01f, 0.f, 0.f));
    p.setEnvironment(environment);

    do {
        c.writePixel(int(p.position.x), int(p.position.y), c1);
        p.tick();
        
    } while (p.position.y > 0.f);

    c.writeToPPM("/home/ilia/dev/gamedev/sdl_game/rayTracer/tests/test.ppm", true);
    return 0;
}
