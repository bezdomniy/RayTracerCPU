#include "material.h"

Material::Material()
{
  this->colour = glm::dvec3(0.0);
  this->ambient = 0.1;
  this->diffuse = 0.9;
  this->specular = 0.9;
  this->shininess = 200;
  this->reflective = 0;
  this->transparency = 0;
  this->refractiveIndex = 1.0;
  this->shadow = true;
}

// TODO make 2 material constructors, 1 with colour, the other with pattern
Material::Material(glm::dvec3 colour, double ambient, double diffuse,
                   double specular, double shininess)
{
  this->colour = colour;
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shininess = shininess;
  this->reflective = 0.;
  this->transparency = 0.;
  this->refractiveIndex = 1.0;
  this->shadow = true;
}

Material::Material(const Material &material)
{
  this->pattern = material.pattern;

  this->colour = material.colour;
  this->ambient = material.ambient;
  this->diffuse = material.diffuse;
  this->specular = material.specular;
  this->shininess = material.shininess;
  this->reflective = material.reflective;
  this->transparency = material.transparency;
  this->refractiveIndex = material.refractiveIndex;

  this->shadow = material.shadow;
}

Material::~Material() {}

void Material::setPattern(std::shared_ptr<Pattern> &pattern)
{
  // this->pattern = std::move(pattern);
  this->pattern = pattern;
}