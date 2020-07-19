#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <string>

#include <cereal/archives/binary.hpp>
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
  virtual ~Pattern();

  virtual glm::dvec3 patternAt(glm::dvec4 point) = 0;
  void setTransform(glm::dmat4 transform);
  void multiplyTransform(glm::dmat4 &transform);
  void calculateInverseTranform();

  glm::dmat4 transform;
  glm::dmat4 inverseTransform;
  // virtual std::string type();
};

class ColourPattern : public Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::base_class<Pattern>(this), colourA, colourB);
  }

public:
  ColourPattern();
  ColourPattern(const ColourPattern &colouredPattern);
  ColourPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  virtual ~ColourPattern();

  glm::dvec3 colourA;
  glm::dvec3 colourB;

  void setColour(glm::dvec3 colour, int index);
};

CEREAL_REGISTER_TYPE(ColourPattern);