#include "material.h"

Material::Material() {}

Material::Material(glm::vec3 colour, float ambient, float diffuse,
                   float specular, float shininess)
{
  this->colour = colour;
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shininess = shininess;
  this->reflective = 0;
  this->transparency = 0;
  this->refractiveIndex = 0;
}

Material::~Material() {}

void Material::setPattern(std::shared_ptr<Pattern> &pattern)
{
  this->pattern = pattern;
}