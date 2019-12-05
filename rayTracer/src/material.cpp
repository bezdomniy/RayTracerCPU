#include "material.h"

Material::Material() {}

// TODO make 2 material constructors, 1 with colour, the other with pattern
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
  this->refractiveIndex = 1.f;
}

Material::~Material() {}

void Material::setPattern(std::shared_ptr<Pattern> &pattern)
{
  this->pattern = pattern;
}