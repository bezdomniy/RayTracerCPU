#pragma once
#define NOISE_STATIC 1

#include "pattern.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <noise/noise.h>
#elif __LINUX__
#include <libnoise/noise.h>
#endif

class StripedPattern : public Pattern
{
private:
  /* data */
public:
  StripedPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~StripedPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  glm::vec3 colourA;
  glm::vec3 colourB;
};

class GradientPattern : public Pattern
{
private:
  /* data */
public:
  GradientPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~GradientPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  glm::vec3 colourA;
  glm::vec3 colourB;
};

class RingPattern : public Pattern
{
private:
  /* data */
public:
  RingPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~RingPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  glm::vec3 colourA;
  glm::vec3 colourB;
};

class CheckedPattern : public Pattern
{
private:
  /* data */
public:
  CheckedPattern(glm::vec3 colourA, glm::vec3 colourB);
  ~CheckedPattern();
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  glm::vec3 colourA;
  glm::vec3 colourB;
};

class BlendedPattern : public Pattern
{
private:
  /* data */
public:
  BlendedPattern(std::shared_ptr<Pattern> &patternA,
                 std::shared_ptr<Pattern> &patternB);
  ~BlendedPattern();

  // void addPattern(std::shared_ptr<Pattern>& pattern);
  virtual glm::vec3 patternAt(glm::vec4 point) override;

  std::shared_ptr<Pattern> patternA;
  std::shared_ptr<Pattern> patternB;
};

class PerturbedPattern : public Pattern
{
private:
  /* data */
public:
  PerturbedPattern(std::shared_ptr<Pattern> &pattern);
  ~PerturbedPattern();

  virtual glm::vec3 patternAt(glm::vec4 point) override;

  std::shared_ptr<Pattern> pattern;
};