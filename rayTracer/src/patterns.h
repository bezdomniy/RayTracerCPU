#pragma once
#define NOISE_STATIC 1

#include "pattern.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include <vector>

// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) ||
// defined(__NT__) #include <noise/noise.h> #elif __unix__
#include <libnoise/noise.h>
// #endif

class StripedPattern : public ColourPattern {
private:
  /* data */
public:
  StripedPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~StripedPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;
  // virtual void setColour(glm::vec3 colour, int index) override;
};

class GradientPattern : public ColourPattern {
private:
  /* data */
public:
  GradientPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~GradientPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;
  // virtual void setColour(glm::vec3 colour, int index) override;
};

class RingPattern : public ColourPattern {
private:
  /* data */
public:
  RingPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~RingPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;
  // virtual void setColour(glm::vec3 colour, int index) override;
};

class CheckedPattern : public ColourPattern {
private:
  /* data */
public:
  CheckedPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~CheckedPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;
  // virtual void setColour(glm::vec3 colour, int index) override;
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
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  std::shared_ptr<Pattern> patternA;
  std::shared_ptr<Pattern> patternB;
};

class PerturbedPattern : public Pattern {
private:
  /* data */
  float perturbedCoeff;

public:
  PerturbedPattern(std::shared_ptr<Pattern> &pattern, float perturbedCoeff);
  PerturbedPattern(const PerturbedPattern &perturbedPattern);
  ~PerturbedPattern();

  virtual glm::vec3 patternAt(glm::vec4 point) override;

  std::shared_ptr<Pattern> pattern;
};