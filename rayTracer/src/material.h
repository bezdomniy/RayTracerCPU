#pragma once

#include "patterns.h"
#include <glm/glm.hpp>
#include <memory>

class Material {
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
  float transparency;
  float refractiveIndex;

  std::shared_ptr<Pattern> pattern;
  void setPattern(std::shared_ptr<Pattern> &pattern);
};
