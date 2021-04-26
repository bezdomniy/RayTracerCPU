#pragma once

#include "patterns.h"
#include "types.h"
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
  Material(Vec3 colour, Vec3 emissiveness, Float ambient, Float diffuse, Float specular,
           Float shininess);

  Material(const Material &material);

  ~Material();

  Vec3 colour;
  Vec3 emissiveness;
  Float ambient;
  Float diffuse;
  Float specular;
  Float shininess;
  Float reflective;
  Float transparency;
  Float refractiveIndex;

  std::shared_ptr<Pattern> pattern;
  bool shadow; //TODO find way to remove
  void setPattern(std::shared_ptr<Pattern> &pattern);
};
