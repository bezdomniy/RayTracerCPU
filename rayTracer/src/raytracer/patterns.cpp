#include "patterns.h"

StripedPattern::StripedPattern(Vec3 colourA, Vec3 colourB)
    : ColourPattern(colourA, colourB) {}

StripedPattern::~StripedPattern() {}

Vec3 StripedPattern::patternAt(const Vec4 &point)
{
  if ((int)(std::floor(point.x)) % 2 == 0)
    return this->colourA;
  return this->colourB;
}

GradientPattern::GradientPattern(Vec3 colourA, Vec3 colourB)
    : ColourPattern(colourA, colourB) {}

GradientPattern::~GradientPattern() {}

Vec3 GradientPattern::patternAt(const Vec4 &point)
{
  Vec3 distance = this->colourB - this->colourA;
  Float fraction = point.x - std::floor(point.x);

  return this->colourA + distance * fraction;
}

RingPattern::RingPattern(Vec3 colourA, Vec3 colourB)
    : ColourPattern(colourA, colourB) {}

RingPattern::~RingPattern() {}

Vec3 RingPattern::patternAt(const Vec4 &point)
{
  if ((int)std::floor(std::sqrt(point.x * point.x + point.z * point.z)) % 2 ==
      0)
    return this->colourA;
  return this->colourB;
}

CheckedPattern::CheckedPattern(Vec3 colourA, Vec3 colourB)
    : ColourPattern(colourA, colourB) {}

CheckedPattern::~CheckedPattern() {}

Vec3 CheckedPattern::patternAt(const Vec4 &point)
{
  if (((int)(std::floor(point.x) + std::floor(point.y) + std::floor(point.z))) % 2 == 0)
    return this->colourA;
  return this->colourB;
}

BlendedPattern::BlendedPattern(std::shared_ptr<Pattern> &patternA,
                               std::shared_ptr<Pattern> &patternB)
    : Pattern()
{
  this->patternA = patternA;
  this->patternB = patternB;
}

BlendedPattern::BlendedPattern(const BlendedPattern &blendedPattern)
    : Pattern()
{
  this->patternA = blendedPattern.patternA;
  this->patternB = blendedPattern.patternB;
}

BlendedPattern::~BlendedPattern() {}

Vec3 BlendedPattern::patternAt(const Vec4 &point)
{
  Mat4 patternTransformA(patternA->inverseTransform);
  Vec4 patternPointA = patternTransformA * point;

  Mat4 patternTransformB(patternB->inverseTransform);
  Vec4 patternPointB = patternTransformB * point;

  return (this->patternA->patternAt(patternPointA) +
          this->patternB->patternAt(patternPointB)) *
         (Float)0.5;
}

PerturbedPattern::PerturbedPattern(std::shared_ptr<Pattern> &pattern,
                                   Float perturbedCoeff)
    : Pattern()
{
  this->pattern = pattern;
  this->perturbedCoeff = perturbedCoeff;
}

PerturbedPattern::PerturbedPattern(const PerturbedPattern &perturbedPattern)
    : Pattern()
{
  this->pattern = perturbedPattern.pattern;
  this->perturbedCoeff = perturbedPattern.perturbedCoeff;
}

PerturbedPattern::~PerturbedPattern() {}

// TODO something to do with this is breaking checkered uv map
Vec3 PerturbedPattern::patternAt(const Vec4 &point)
{
  Mat4 patternTransform(pattern->inverseTransform);
  Vec4 patternPoint = patternTransform * point;

  Float value = SimplexNoise::noise(patternPoint.x * perturbedCoeff,
                                    patternPoint.y * perturbedCoeff,
                                    patternPoint.z * perturbedCoeff);

  patternPoint.x += value;
  patternPoint.y += value;
  patternPoint.z += value;

  return this->pattern->patternAt(patternPoint);
}

MappedPattern::MappedPattern(std::shared_ptr<UVTexture> &uvTexture, std::shared_ptr<TextureMap> &textureMap)
    : Pattern()
{
  this->uvTexture = uvTexture;
  this->textureMap = textureMap;
}

MappedPattern::MappedPattern(const MappedPattern &mappedPattern)
    : Pattern()
{
  this->uvTexture = mappedPattern.uvTexture;
  this->textureMap = mappedPattern.textureMap;
}

MappedPattern::~MappedPattern() {}

Vec3 MappedPattern::patternAt(const Vec4 &point)
{
  // Vec2 uv = this->textureMap->uv_map(point);
  auto uv = this->textureMap->uv_map(point);

  return this->uvTexture->patternAt(uv.first, uv.second);
}
