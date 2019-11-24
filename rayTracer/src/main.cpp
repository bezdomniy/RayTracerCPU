#include "camera.h"
#include "canvas.h"
#include "geometry.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "plane.h"
#include "world.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char const *argv[])
{
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(
        glm::vec4(0.f, 1.5f, -5.f, 1.f), glm::vec4(0.f, 1.f, 0.f, 1.f),
        glm::vec4(0.f, 1.f, 0.f, 0.f), 500.f, 250.f, glm::radians(60.f));

    // Sphere s(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    // Sphere s2(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    // Sphere s3(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);

    std::shared_ptr<Shape> s = std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    std::shared_ptr<Shape> s2 = std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    std::shared_ptr<Shape> s3 = std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);

    std::shared_ptr<Shape> floor = std::make_shared<Plane>(0, glm::vec4(0.f, 0.f, 0.f, 1.f));
    std::shared_ptr<Shape> backWall = std::make_shared<Plane>(0, glm::vec4(0.f, 0.f, 0.f, 1.f));

    float ambient = 0.1f;
    float diffuse = 0.7f;
    float specular = 0.3f;
    float shininess = 200.f;
    std::shared_ptr<Material> material = std::make_shared<Material>(
        glm::vec3(1.f, 0.2f, 1.f), ambient, diffuse, specular, shininess);
    std::shared_ptr<Material> material2 = std::make_shared<Material>(
        glm::vec3(0.5f, 1.f, 0.1f), ambient, diffuse, specular, shininess);
    std::shared_ptr<Material> material3 = std::make_shared<Material>(
        glm::vec3(1.f, 0.8f, 0.1f), ambient, diffuse, specular, shininess);
    std::shared_ptr<Material> materialFloor = std::make_shared<Material>(
        glm::vec3(1.f, 0.9f, 0.9f), ambient, diffuse, specular, shininess);
    s->setMaterial(material);
    s2->setMaterial(material2);
    s3->setMaterial(material3);
    floor->setMaterial(materialFloor);
    backWall->setMaterial(materialFloor);
	//test
    s->transform = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, 1.f, 0.5f));
    s->transform *= glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 1.f));
    // s.transform = glm::scale(glm::mat4(1.f), glm::vec3(9.f, 9.f, 9.f));
    s2->transform = glm::translate(glm::mat4(1.f), glm::vec3(1.5f, 0.5f, -0.5f));
    s2->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.5f));

    s3->transform =
        glm::translate(glm::mat4(1.f), glm::vec3(-1.5f, 0.33f, -0.75f));
    s3->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.33f, 0.33f, 0.33f));

	// floor->transform *= glm::rotate(glm::mat4(1.f), glm::pi<float>()/2, glm::vec3(1.f, 0.f, 0.f));
	backWall->transform *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 2.5f));
    backWall->transform *= glm::rotate(glm::mat4(1.f), glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f));
    // backWall->transform *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 5.f));
    // left.transform ← translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33)

    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
        0, glm::vec4(-10.f, 10.f, -10.f, 1.f), glm::vec3(1.f, 1.f, 1.f));

    World world;
    world.addShape(s);
    world.addShape(s2);
    world.addShape(s3);
    world.addShape(floor);
    world.addShape(backWall);
    world.addLight(light);

    Renderer renderer(camera);
    renderer.render(world);

    renderer.canvas.writeToPPM("./test.ppm", false);

    return 0;
}
