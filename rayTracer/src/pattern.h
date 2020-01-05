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

  virtual glm::vec3 patternAt(glm::vec4 point) = 0;
  void setTransform(glm::mat4 transform);
  void multiplyTransform(glm::mat4 &transform);
  void calculateInverseTranform();

  glm::mat4 transform;
  glm::mat4 inverseTransform;
  // virtual std::string type();
};

class ColourPattern : public Pattern {
private:
  /* data */

public:
  ColourPattern();
  ColourPattern(const ColourPattern &colouredPattern);
  ColourPattern(glm::vec3 colourA, glm::vec3 colourB);
  virtual ~ColourPattern();

  glm::vec3 colourA;
  glm::vec3 colourB;

  // virtual glm::vec3 patternAt(glm::vec4 point) = 0;
  // virtual std::string type() override;

  void setColour(glm::vec3 colour, int index);
};
