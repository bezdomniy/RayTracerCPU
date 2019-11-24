#include "material.h"

Material::Material() {}

Material::Material(glm::vec3 colour, float ambient, float diffuse,
                   float specular, float shininess) {
  this->colour = colour;
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shininess = shininess;
}

Material::~Material() {}

void Material::setPattern(std::shared_ptr<Pattern>& pattern) {
  this->pattern = pattern;
}