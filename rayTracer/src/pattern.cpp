#include "pattern.h"

Pattern::Pattern() {
  this->transform = glm::dmat4(1.0);
  this->inverseTransform = glm::dmat4(1.0);
}

Pattern::Pattern(const Pattern &pattern) {
  this->transform = pattern.transform;
  this->inverseTransform = pattern.inverseTransform;
}

void Pattern::setTransform(glm::dmat4 transform) {
  this->transform = transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Pattern::multiplyTransform(glm::dmat4 &transform) {
  this->transform = transform * this->transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Pattern::calculateInverseTranform() {
  this->inverseTransform = glm::affineInverse(this->transform);
}

Pattern::~Pattern() {}

ColourPattern::ColourPattern() : Pattern() {}

ColourPattern::ColourPattern(glm::dvec3 colourA, glm::dvec3 colourB) : Pattern() {
  this->colourA = colourA;
  this->colourB = colourB;
}

ColourPattern::ColourPattern(const ColourPattern &colouredPattern) : Pattern() {
  this->colourA = colouredPattern.colourA;
  this->colourB = colouredPattern.colourB;
}

void ColourPattern::setColour(glm::dvec3 colour, int index) {
  if (index == 0)
    this->colourA = colour;
  else if (index == 1)
    this->colourB = colour;
}

ColourPattern::~ColourPattern() {}