#pragma once

#include <glm/glm.hpp>

class Pattern
{
private:
  /* data */
public:
  Pattern();
  virtual ~Pattern();

  virtual glm::vec3 patternAt(glm::vec4 point) = 0;
  void setTransform(glm::mat4 transform);
  void multiplyTransform(glm::mat4 &transform);

  glm::mat4 transform;
};

class ColourPattern : public Pattern
{
private:
  /* data */

public:
  ColourPattern();
  ColourPattern(glm::vec3 colourA, glm::vec3 colourB);
  virtual ~ColourPattern();

  glm::vec3 colourA;
  glm::vec3 colourB;

  virtual glm::vec3 patternAt(glm::vec4 point) = 0;

  void setColour(glm::vec3 colour, int index);
};
