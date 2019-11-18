#pragma once

#include "shape.h"

class PointLight
{
private:
  /* data */
public:
  PointLight(unsigned int id, glm::vec4 position, glm::vec3 intensity);
  ~PointLight();

  int id;
  glm::vec4 position;
  glm::vec3 intensity;
};
