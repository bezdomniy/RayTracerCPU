#pragma once

#include "types.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <string>

// #include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>

#include "serialisation.h"

class Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(transform, inverseTransform);
  }

public:
  Pattern();
  Pattern(const Pattern &pattern);
  virtual ~Pattern() = 0;

  virtual Vec3 patternAt(const Vec4 &point) = 0;
  void setTransform(Mat4 transform);
  void multiplyTransform(Mat4 &transform);
  void calculateInverseTranform();

  Mat4 transform;
  Mat4 inverseTransform;
  // virtual std::string type();
};

class ColourPattern : public Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Pattern>(this), colourA, colourB);
  }

public:
  ColourPattern();
  ColourPattern(const ColourPattern &colouredPattern);
  ColourPattern(Vec3 colourA, Vec3 colourB);
  virtual ~ColourPattern() = 0;

  Vec3 colourA;
  Vec3 colourB;

  void setColour(Vec3 colour, int index);
};

CEREAL_REGISTER_TYPE(ColourPattern);