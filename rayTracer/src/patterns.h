#pragma once

#include "../include/SimplexNoise.h"
#include "pattern.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include <vector>

class StripedPattern : public ColourPattern {
private:
  /* data */
public:
  StripedPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~StripedPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
  // virtual void setColour(glm::dvec3 colour, int index) override;
};

class GradientPattern : public ColourPattern {
private:
  /* data */
public:
  GradientPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~GradientPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
  // virtual void setColour(glm::dvec3 colour, int index) override;
};

class RingPattern : public ColourPattern {
private:
  /* data */
public:
  RingPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~RingPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
  // virtual void setColour(glm::dvec3 colour, int index) override;
};

class CheckedPattern : public ColourPattern {
private:
  /* data */
public:
  CheckedPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~CheckedPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
  // virtual void setColour(glm::dvec3 colour, int index) override;
};

class BlendedPattern : public Pattern {
private:
  /* data */
public:
  BlendedPattern(std::shared_ptr<Pattern> &patternA,
                 std::shared_ptr<Pattern> &patternB);
  BlendedPattern(const BlendedPattern &blendedPattern);
  ~BlendedPattern();

  // void addPattern(std::shared_ptr<Pattern>& pattern);
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;

  std::shared_ptr<Pattern> patternA;
  std::shared_ptr<Pattern> patternB;
};

class PerturbedPattern : public Pattern {
private:
  /* data */
  double perturbedCoeff;

public:
  PerturbedPattern(std::shared_ptr<Pattern> &pattern, double perturbedCoeff);
  PerturbedPattern(const PerturbedPattern &perturbedPattern);
  ~PerturbedPattern();

  virtual glm::dvec3 patternAt(glm::dvec4 point) override;

  std::shared_ptr<Pattern> pattern;
};