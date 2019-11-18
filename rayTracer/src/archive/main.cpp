#include "environment.h"
#include "projectile.h"
#include "../canvas.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

void drawCircle()
{
    Canvas c(100, 100);

    glm::vec3 c1(1.f, 0.f, 0.f);

    glm::vec4 point(40.f, 0.f, 0.f, 1.f);

    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(50.0f, 50.0f, 0.0f));
    glm::mat4 invTranslation = glm::affineInverse(translation);

    glm::quat quatRotation =
        glm::angleAxis(glm::radians(30.f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation = glm::mat4_cast(quatRotation);

    std::cout << point.x << " " << point.y << std::endl;
    for (int i = 0; i < 12; i++)
    {
        point = translation * rotation * point;

        c.writePixel(int(point.x), int(point.y), c1);
        point = invTranslation * point;
    }

    c.writeToPPM("./test.ppm", true);
}

void drawProjectile()
{
    Canvas c(900, 550);

    glm::vec3 c1(1.f, 0.f, 0.f);
    glm::vec3 c2(0.f, 0.f, 0.f);

    c.clear(c2);

    Projectile p(glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(1.f, 1.8f, 0.f, 0.f),
                 11.25f);

    std::unique_ptr<Environment> environment = std::make_unique<Environment>(
        glm::vec4(0.f, -0.1f, 0.f, 0.f), glm::vec4(-0.01f, 0.f, 0.f, 0.f));
    p.setEnvironment(environment);

    do
    {
        c.writePixel(int(p.position.x), int(p.position.y), c1);
        p.tick();

    } while (p.position.y > 0.f);

    // c.writeToPPM("/home/ilia/dev/gamedev/sdl_game/rayTracer/tests/test.ppm",
    // true);
    c.writeToPPM("./test.ppm", true);
}