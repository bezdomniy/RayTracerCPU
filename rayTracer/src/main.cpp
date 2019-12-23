#include "camera.h"
#include "canvas.h"
#include "geometry.h"
#include "pattern.h"
#include "patterns.h"
#include "plane.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{

    // Scene 1
    // std::shared_ptr<Camera> camera = std::make_shared<Camera>(0,
    //                                                           glm::vec4(0.f, 1.5f, -5.f, 1.f), glm::vec4(0.f, 1.f, 0.f, 1.f),
    //                                                           glm::vec4(0.f, 1.f, 0.f, 0.f), 500.f, 250.f, glm::radians(60.f));

    // std::shared_ptr<Shape> s =
    //     std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    // std::shared_ptr<Shape> s2 =
    //     std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    // std::shared_ptr<Shape> s3 =
    //     std::make_shared<Sphere>(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);

    // std::shared_ptr<Shape> floor =
    //     std::make_shared<Plane>(0, glm::vec4(0.f, 0.f, 0.f, 1.f));
    // std::shared_ptr<Shape> backWall =
    //     std::make_shared<Plane>(0, glm::vec4(0.f, 0.f, 0.f, 1.f));

    // float ambient = 0.1f;
    // float diffuse = 0.7f;
    // float specular = 0.3f;
    // float shininess = 200.f;
    // std::shared_ptr<Material> material = std::make_shared<Material>(
    //     glm::vec3(1.f, 0.2f, 1.f), ambient, diffuse, specular, shininess);
    // std::shared_ptr<Material> material2 = std::make_shared<Material>(
    //     glm::vec3(0.5f, 1.f, 0.1f), ambient, diffuse, specular, shininess);
    // std::shared_ptr<Material> material3 = std::make_shared<Material>(
    //     glm::vec3(1.f, 0.8f, 0.1f), ambient, diffuse, specular, shininess);
    // std::shared_ptr<Material> materialFloor = std::make_shared<Material>(
    //     glm::vec3(1.f, 0.9f, 0.9f), ambient, diffuse, specular, shininess);
    // std::shared_ptr<Material> materialBackWall = std::make_shared<Material>(
    //     glm::vec3(1.f, 0.9f, 0.9f), ambient, diffuse, specular, shininess);

    // std::shared_ptr<Pattern> stripes = std::make_shared<StripedPattern>(
    //     glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    // std::shared_ptr<Pattern> rings = std::make_shared<RingPattern>(
    //     glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    // std::shared_ptr<Pattern> gradient = std::make_shared<GradientPattern>(
    //     glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    // std::shared_ptr<Pattern> checks = std::make_shared<CheckedPattern>(
    //     glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

    // rings->transform *=
    //     glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    // rings->transform *=
    //     glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.25f, 0.25f));
    // stripes->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.25f, 0.25f));

    // std::shared_ptr<Pattern> blended =
    //     std::make_shared<BlendedPattern>(rings, stripes);

    // std::shared_ptr<Pattern> perturbed =
    //     std::make_shared<PerturbedPattern>(stripes);

    // // material->setPattern(perturbed);
    // materialFloor->setPattern(checks);
    // materialBackWall->setPattern(checks);

    // s->setMaterial(material);
    // s2->setMaterial(material2);
    // s3->setMaterial(material3);
    // floor->setMaterial(materialFloor);
    // backWall->setMaterial(materialBackWall);

    // floor->material->reflective = 0.0f;
    // s->material->reflective = .9f;
    // // s->material->refractiveIndex = 1.52f;
    // // s->material->transparency = 1.f;

    // s->transform = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, 1.f, 0.5f));
    // s->transform *= glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 1.f));
    // // s.transform = glm::scale(glm::mat4(1.f), glm::vec3(9.f, 9.f, 9.f));
    // s2->transform = glm::translate(glm::mat4(1.f), glm::vec3(1.5f, 0.5f, -0.5f));
    // s2->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.5f));

    // s3->transform =
    //     glm::translate(glm::mat4(1.f), glm::vec3(-1.5f, 0.33f, -0.75f));
    // s3->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.33f, 0.33f, 0.33f));

    // backWall->transform *=
    //     glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 2.5f));
    // backWall->transform *= glm::rotate(glm::mat4(1.f), glm::radians(90.f),
    //                                    glm::vec3(1.f, 0.f, 0.f));

    // std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
    //     0, glm::vec4(-10.f, 10.f, -10.f, 1.f), glm::vec3(1.f, 1.f, 1.f));

    // World world;
    // world.addShape(s);
    // world.addShape(s2);
    // world.addShape(s3);
    // world.addShape(floor);
    // world.addShape(backWall);
    // world.addLight(light);

    // Scene 2
    // std::shared_ptr<Camera> camera = std::make_shared<Camera>(
    //     glm::vec4(0.f, 0.f, -5.f, 1.f), glm::vec4(0.f, 0.f, 0.f, 1.f),
    //     glm::vec4(0.f, 1.f, 0.f, 0.f), 1024.f, 1024.f, glm::radians(45.f));

    // std::shared_ptr<Shape> floor =
    //     std::make_shared<Plane>(33, glm::vec4(0.f, 0.f, 0.f, 1.f));

    // floor->transform *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 10.f));
    // floor->transform *= glm::rotate(glm::mat4(1.f),1.5708f, glm::vec3(1.f, 0.f, 0.f));

    // std::shared_ptr<Material> materialFloor = std::make_shared<Material>(
    //     glm::vec3(1.f, 0.2f, 1.f), 0.8f, 0.2f, 0.f, 0.f);

    // std::shared_ptr<Pattern> checks = std::make_shared<CheckedPattern>(
    //     glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.85f, 0.85f, 0.85f));
    // // checks->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.25f, 0.25f));

    // materialFloor->setPattern(checks);
    // floor->setMaterial(materialFloor);

    // std::shared_ptr<Shape> s =
    //     std::make_shared<Sphere>(11, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);

    // std::shared_ptr<Shape> s2 =
    //     std::make_shared<Sphere>(22, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    // s2->transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.5f));

    // std::shared_ptr<Material> materialS = std::make_shared<Material>(
    //     glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 0.9f, 300.f);

    // std::shared_ptr<Material> materialS2 = std::make_shared<Material>(
    //     glm::vec3(0.f, 0.f, 0.f), 0.f, 0.f, 0.9f, 300.f);

    // materialS->transparency = 0.9f;
    // materialS->reflective = 0.9f;
    // materialS->refractiveIndex = 1.52f;

    // materialS2->transparency = 0.9f;
    // materialS2->reflective = 0.9f;
    // materialS2->refractiveIndex = 1.0000034f;

    // s->setMaterial(materialS);
    // s2->setMaterial(materialS2);

    // std::shared_ptr<PointLight> light = std::make_shared<PointLight>(
    //     0, glm::vec4(2.f, 10.f, -5.f, 1.f), glm::vec3(0.9f, 0.9f, 0.9f));

    // World world;
    // world.addShape(floor);
    // world.addShape(s);
    // world.addShape(s2);

    // world.addLight(light);

    World world;

    std::shared_ptr<Camera> camera = world.loadFromFile("./src/scene2.yaml");

    Renderer renderer(camera);
    renderer.render(world);
    renderer.canvas.writeToPPM("./test.ppm", false);

    return 0;
}
