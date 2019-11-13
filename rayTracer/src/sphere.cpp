#include "sphere.h"

Sphere::Sphere(unsigned int id, glm::vec4 position, float radius): Shape(id, position)
{
    this->radius = radius;
    this->transform = glm::mat4(1.f);
}

Sphere::~Sphere()
{
}

void Sphere::setMaterial(std::shared_ptr<Material> mat) {
    this->material = mat;
}

glm::vec4 Sphere::normalAt(glm::vec4 point) {
    glm::mat4 transformInverse(glm::affineInverse(this->transform));
    glm::vec4 objectPoint = transformInverse * point;
    glm::vec4 objectNormal = objectPoint - glm::vec4(0.f,0.f,0.f,1.f);
    glm::vec4 worldNormal = glm::transpose(transformInverse) * objectNormal;
    worldNormal.w = 0.f;

    return glm::normalize(worldNormal);
}