#pragma once

#include <glm/glm.hpp>

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
};
