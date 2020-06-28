#pragma once

#include "../include/SimplexNoise.h"
#include "pattern.h"
#include "textureMap.h"
#include "uvTextures.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include <vector>

class StripedPattern : public ColourPattern
{
public:
  StripedPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~StripedPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
};

class GradientPattern : public ColourPattern
{
public:
  GradientPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~GradientPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
};

class RingPattern : public ColourPattern
{
public:
  RingPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~RingPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
};

class CheckedPattern : public ColourPattern
{
public:
  CheckedPattern(glm::dvec3 colourA, glm::dvec3 colourB);
  ~CheckedPattern();
  virtual glm::dvec3 patternAt(glm::dvec4 point) override;
};

class BlendedPattern : public Pattern
{
public:
  BlendedPattern(std::shared_ptr<Pattern> &patternA,
                 std::shared_ptr<Pattern> &patternB);
  BlendedPattern(const BlendedPattern &blendedPattern);
  ~BlendedPattern();

  virtual glm::dvec3 patternAt(glm::dvec4 point) override;

  std::shared_ptr<Pattern> patternA;
  std::shared_ptr<Pattern> patternB;
};

class PerturbedPattern : public Pattern
{
public:
  PerturbedPattern(std::shared_ptr<Pattern> &pattern, double perturbedCoeff);
  PerturbedPattern(const PerturbedPattern &perturbedPattern);
  ~PerturbedPattern();

  virtual glm::dvec3 patternAt(glm::dvec4 point) override;

  std::shared_ptr<Pattern> pattern;
  double perturbedCoeff;
};

class MappedPattern : public Pattern
{
public:
  MappedPattern(std::shared_ptr<UVTexture> &uvTexture, std::shared_ptr<TextureMap> &textureMap);
  MappedPattern(const MappedPattern &mappedPattern);
  ~MappedPattern();

  virtual glm::dvec3 patternAt(glm::dvec4 point) override;

  std::shared_ptr<UVTexture> uvTexture;
  std::shared_ptr<TextureMap> textureMap;
};