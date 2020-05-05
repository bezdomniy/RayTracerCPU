#include "patterns.h"

StripedPattern::StripedPattern(glm::dvec3 colourA, glm::dvec3 colourB)
    : ColourPattern(colourA, colourB) {}

StripedPattern::~StripedPattern() {}

glm::dvec3 StripedPattern::patternAt(glm::dvec4 point) {
  if ((int)(std::floor(point.x)) % 2 == 0)
    return this->colourA;
  return this->colourB;
}

GradientPattern::GradientPattern(glm::dvec3 colourA, glm::dvec3 colourB)
    : ColourPattern(colourA, colourB) {}

GradientPattern::~GradientPattern() {}

glm::dvec3 GradientPattern::patternAt(glm::dvec4 point) {
  glm::dvec3 distance = this->colourB - this->colourA;
  double fraction = point.x - std::floor(point.x);

  return this->colourA + distance * fraction;
}

RingPattern::RingPattern(glm::dvec3 colourA, glm::dvec3 colourB)
    : ColourPattern(colourA, colourB) {}

RingPattern::~RingPattern() {}

glm::dvec3 RingPattern::patternAt(glm::dvec4 point) {
  if ((int)std::floor(std::sqrt(point.x * point.x + point.z * point.z)) % 2 ==
      0)
    return this->colourA;
  return this->colourB;
}

CheckedPattern::CheckedPattern(glm::dvec3 colourA, glm::dvec3 colourB)
    : ColourPattern(colourA, colourB) {}

CheckedPattern::~CheckedPattern() {}

glm::dvec3 CheckedPattern::patternAt(glm::dvec4 point) {
  if (((int)(std::floor(point.x) + std::floor(point.y) + std::floor(point.z))) % 2 == 0)
    return this->colourA;
  return this->colourB;
}

BlendedPattern::BlendedPattern(std::shared_ptr<Pattern> &patternA,
                               std::shared_ptr<Pattern> &patternB)
    : Pattern() {
  this->patternA = patternA;
  this->patternB = patternB;
}

BlendedPattern::BlendedPattern(const BlendedPattern &blendedPattern)
    : Pattern() {
  this->patternA = blendedPattern.patternA;
  this->patternB = blendedPattern.patternB;
}

BlendedPattern::~BlendedPattern() {}

glm::dvec3 BlendedPattern::patternAt(glm::dvec4 point) {
  glm::dmat4 patternTransformA(glm::affineInverse(patternA->transform));
  glm::dvec4 patternPointA = patternTransformA * point;

  glm::dmat4 patternTransformB(glm::affineInverse(patternB->transform));
  glm::dvec4 patternPointB = patternTransformB * point;

  return (this->patternA->patternAt(patternPointA) +
          this->patternB->patternAt(patternPointB)) *
         0.5;
}

PerturbedPattern::PerturbedPattern(std::shared_ptr<Pattern> &pattern,
                                   double perturbedCoeff)
    : Pattern() {
  this->pattern = pattern;
  this->perturbedCoeff = perturbedCoeff;
}

PerturbedPattern::PerturbedPattern(const PerturbedPattern &perturbedPattern)
    : Pattern() {
  this->pattern = perturbedPattern.pattern;
  this->perturbedCoeff = perturbedPattern.perturbedCoeff;
}

PerturbedPattern::~PerturbedPattern() {}

glm::dvec3 PerturbedPattern::patternAt(glm::dvec4 point) {
  glm::dmat4 patternTransform(glm::affineInverse(pattern->transform));
  glm::dvec4 patternPoint = patternTransform * point;

  double value = SimplexNoise::noise(patternPoint.x * perturbedCoeff,
                                    patternPoint.y * perturbedCoeff,
                                    patternPoint.z * perturbedCoeff);

  patternPoint.x += value;
  patternPoint.y += value;
  patternPoint.z += value;

  return this->pattern->patternAt(patternPoint);
}

MappedPattern::MappedPattern(std::shared_ptr<UVTexture> &uvTexture, std::shared_ptr<TextureMap> &textureMap)
    : Pattern() {
  this->uvTexture = uvTexture;
  this->textureMap = textureMap;
}

MappedPattern::MappedPattern(const MappedPattern &mappedPattern)
    : Pattern() {
  this->uvTexture = mappedPattern.uvTexture;
  this->textureMap = mappedPattern.textureMap;
}

MappedPattern::~MappedPattern() {}

glm::dvec3 MappedPattern::patternAt(glm::dvec4 point) {
  glm::dvec2 uv = this->textureMap->uv_map(point);
  return this->uvTexture->patternAt(uv);
}
