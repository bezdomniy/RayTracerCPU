#include "pointLight.h"

PointLight::PointLight(unsigned int id, glm::vec4 position, glm::vec3 intensity)
{

  this->position = position;
  this->intensity = intensity;
}

PointLight::~PointLight() {}