#pragma once

#include "patterns.h"
#include <glm/glm.hpp>
#include <memory>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

class Material
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(colour,
            emissiveness,
            ambient,
            diffuse,
            specular,
            shininess,
            reflective,
            transparency,
            refractiveIndex,
            shadow,
            pattern);
  }

public:
  Material();
  Material(glm::dvec3 colour, glm::dvec3 emissiveness, double ambient, double diffuse, double specular,
           double shininess);

  Material(const Material &material);

  ~Material();

  glm::dvec3 colour;
  glm::dvec3 emissiveness;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
  double reflective;
  double transparency;
  double refractiveIndex;

  std::shared_ptr<Pattern> pattern;
  bool shadow; //TODO find way to remove
  void setPattern(std::shared_ptr<Pattern> &pattern);
};
