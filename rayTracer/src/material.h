#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "pattern.h"

class Material
{
private:
  /* data */
public:
  Material();
  Material(glm::vec3 colour, float ambient, float diffuse, float specular,
           float shininess);

  ~Material();

  glm::vec3 colour;
  float ambient;
  float diffuse;
  float specular;
  float shininess;
  float reflective;

  std::shared_ptr<Pattern> pattern;
  void setPattern(std::shared_ptr<Pattern> &pattern);
};
