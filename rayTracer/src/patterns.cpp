#include "patterns.h"

StripedPattern::StripedPattern(glm::vec3 colourA, glm::vec3 colourB)
    : Pattern() {
  this->colourA = colourA;
  this->colourB = colourB;
}

StripedPattern::~StripedPattern() {}

glm::vec3 StripedPattern::patternAt(glm::vec4 point) {
  if ((int)(std::floor(point.x)) % 2 == 0)
    return this->colourA;
  return this->colourB;
}

GradientPattern::GradientPattern(glm::vec3 colourA, glm::vec3 colourB)
    : Pattern() {
  this->colourA = colourA;
  this->colourB = colourB;
}

GradientPattern::~GradientPattern() {}

glm::vec3 GradientPattern::patternAt(glm::vec4 point) {
  glm::vec3 distance = this->colourB - this->colourA;
  float fraction = point.x - std::floor(point.x);

  return this->colourA + distance * fraction;
}

RingPattern::RingPattern(glm::vec3 colourA, glm::vec3 colourB) : Pattern() {
  this->colourA = colourA;
  this->colourB = colourB;
}

RingPattern::~RingPattern() {}

glm::vec3 RingPattern::patternAt(glm::vec4 point) {
  if ((int)std::floor(std::sqrt(point.x * point.x + point.z * point.z)) % 2 ==
      0)
    return this->colourA;
  return this->colourB;
}

CheckedPattern::CheckedPattern(glm::vec3 colourA, glm::vec3 colourB)
    : Pattern() {
  this->colourA = colourA;
  this->colourB = colourB;
}

CheckedPattern::~CheckedPattern() {}

glm::vec3 CheckedPattern::patternAt(glm::vec4 point) {
  if (((int)(std::floor(point.x) + std::floor(point.y) + std::floor(point.z))) %
          2 ==
      0)
    return this->colourA;
  return this->colourB;
}

BlendedPattern::BlendedPattern(std::shared_ptr<Pattern> &patternA,
                               std::shared_ptr<Pattern> &patternB)
    : Pattern() {
  this->patternA = patternA;
  this->patternB = patternB;
}

BlendedPattern::~BlendedPattern() {}

glm::vec3 BlendedPattern::patternAt(glm::vec4 point) {
  glm::mat4 patternTransformA(glm::affineInverse(patternA->transform));
  glm::vec4 patternPointA = patternTransformA * point;

  glm::mat4 patternTransformB(glm::affineInverse(patternB->transform));
  glm::vec4 patternPointB = patternTransformB * point;

  return (this->patternA->patternAt(patternPointA) +
          this->patternB->patternAt(patternPointB)) *
         0.5f;
}

PerturbedPattern::PerturbedPattern(std::shared_ptr<Pattern> &pattern)
    : Pattern() {
  this->pattern = pattern;
}

PerturbedPattern::~PerturbedPattern() {}

glm::vec3 PerturbedPattern::patternAt(glm::vec4 point) {
  noise::module::Perlin perlinModule;

  glm::mat4 patternTransform(glm::affineInverse(pattern->transform));
  glm::vec4 patternPoint = patternTransform * point;

  double value = perlinModule.GetValue(
      patternPoint.x * .5f, patternPoint.y * .5f, patternPoint.z * .5f);

  patternPoint.x += value;
  patternPoint.y += value;
  patternPoint.z += value;

  return this->pattern->patternAt(patternPoint);
}