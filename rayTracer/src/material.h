#pragma once

#include "patterns.h"
#include <glm/glm.hpp>
#include <memory>

class Material
{
private:
  /* data */
public:
  Material();
  Material(glm::dvec3 colour, double ambient, double diffuse, double specular,
           double shininess);

  ~Material();

  glm::dvec3 colour;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
  double reflective;
  double transparency;
  double refractiveIndex;

  bool shadow = true;

  std::shared_ptr<Pattern> pattern;
  void setPattern(std::shared_ptr<Pattern> &pattern);
};
