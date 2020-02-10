#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <string>

class Pattern {
private:
  /* data */
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

class ColourPattern : public Pattern {
private:
  /* data */

public:
  ColourPattern();
  ColourPattern(const ColourPattern &colouredPattern);
  ColourPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  virtual ~ColourPattern();

  glm::dvec3 colourA;
  glm::dvec3 colourB;

  // virtual glm::dvec3 patternAt(glm::dvec4 point) = 0;
  // virtual std::string type() override;

  void setColour(glm::dvec3 colour, int index);
};
