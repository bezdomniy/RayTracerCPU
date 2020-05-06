#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <algorithm>
#include <vector>

class TextureMap
{
public:
    TextureMap(/* args */);
    TextureMap(const TextureMap &textureMap);
    virtual ~TextureMap();

    virtual glm::dvec2 uv_map(glm::dvec4 point) = 0;
};

class SphericalMap: public TextureMap
{
public:
    SphericalMap(/* args */);
    ~SphericalMap();
    virtual glm::dvec2 uv_map(glm::dvec4 point) override;
};

class PlanarMap: public TextureMap
{
public:
    PlanarMap(/* args */);
    ~PlanarMap();
    virtual glm::dvec2 uv_map(glm::dvec4 point) override;
};

class CylinderMap: public TextureMap
{
public:
    CylinderMap(/* args */);
    ~CylinderMap();
    virtual glm::dvec2 uv_map(glm::dvec4 point) override;
};

class CubeMap: public TextureMap
{
public:
    CubeMap(/* args */);
    ~CubeMap();
    virtual glm::dvec2 uv_map(glm::dvec4 point) override;
    int faceFromPoint(glm::dvec4 point);

    glm::dvec2 cubeUVLeft(glm::dvec4 point);
    glm::dvec2 cubeUVRight(glm::dvec4 point);
    glm::dvec2 cubeUVUp(glm::dvec4 point);
    glm::dvec2 cubeUVDown(glm::dvec4 point);
    glm::dvec2 cubeUVFront(glm::dvec4 point);
    glm::dvec2 cubeUVBack(glm::dvec4 point);
};