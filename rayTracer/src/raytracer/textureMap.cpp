#include "textureMap.h"

TextureMap::TextureMap()
{
}

TextureMap::TextureMap(const TextureMap &textureMap)
{
}

TextureMap::~TextureMap()
{
}

SphericalMap::SphericalMap()
{
}

SphericalMap::~SphericalMap()
{
}

int SphericalMap::faceFromPoint(Vec4 point)
{
    return 0;
}

std::pair<Vec2, int> SphericalMap::uv_map(Vec4 point)
{
    Float theta = glm::atan(point.x, point.z);

    Float radius = glm::length(Vec3(point));

    Float phi = glm::acos(point.y / radius);

    Float raw_u = theta / (2 * glm::pi<Float>());
    Float u = 1.0 - (raw_u + 0.5);

    Float v = 1.0 - phi / glm::pi<Float>();

    return {Vec2(u, v), 0};
}

PlanarMap::PlanarMap()
{
}

PlanarMap::~PlanarMap()
{
}

int PlanarMap::faceFromPoint(Vec4 point)
{
    return 0;
}

std::pair<Vec2, int> PlanarMap::uv_map(Vec4 point)
{
    Float u = std::fmod(point.x, 1);
    Float v = std::fmod(point.z, 1);
    return {Vec2(u, v), 0};
}

CylinderMap::CylinderMap()
{
}

CylinderMap::~CylinderMap()
{
}

int CylinderMap::faceFromPoint(Vec4 point)
{
    // TODO update to get caps
    return 0;
}

// #include <iostream>
std::pair<Vec2, int> CylinderMap::uv_map(Vec4 point)
{
    Float theta = glm::atan(point.x, point.z);
    Float raw_u = theta / (2 * glm::pi<Float>());
    Float u = 1.0 - (raw_u + 0.5);

    Float v = std::fmod(point.y, 1);

    // std::cout << u << " | " << v << std::endl;
    // std::cout << point.x << " " << point.y << " " << point.z << " " << std::endl;

    return {Vec2(u, v), 0};
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

std::pair<Vec2, int> CubeMap::uv_map(Vec4 point)
{
    int face = faceFromPoint(point);

    // std::cout << point.x << " " << point.y << " " << point.z << " " << std::endl;

    if (face == 0)
    {
        return {cubeUVRight(point), face};
    }
    else if (face == 1)
    {
        return {cubeUVLeft(point), face};
    }
    else if (face == 2)
    {
        return {cubeUVUp(point), face};
    }
    else if (face == 3)
    {
        return {cubeUVDown(point), face};
    }
    else if (face == 4)
    {
        return {cubeUVFront(point), face};
    }
    else
    {
        return {cubeUVBack(point), face};
    }
}

int CubeMap::faceFromPoint(Vec4 point)
{
    std::vector<Float> coords = {point.x, point.y, point.z};
    std::vector<Float> absCoords = {std::abs(point.x), std::abs(point.y), std::abs(point.z)};

    int maxAbsIndex = std::distance(absCoords.begin(), max_element(absCoords.begin(), absCoords.end()));

    if (coords[maxAbsIndex] < 0)
    {
        return (maxAbsIndex * 2) + 1;
    }
    else
    {
        return (maxAbsIndex * 2);
    }
}

Vec2 CubeMap::cubeUVLeft(Vec4 point)
{
    Float u = std::fmod(point.z + 1., 2.) / 2.;
    Float v = std::fmod(point.y + 1., 2.) / 2.;
    return Vec2(u, v);
}

Vec2 CubeMap::cubeUVRight(Vec4 point)
{
    Float u = std::fmod(1. - point.z, 2.) / 2.;
    Float v = std::fmod(point.y + 1., 2.) / 2.;
    return Vec2(u, v);
}

Vec2 CubeMap::cubeUVUp(Vec4 point)
{
    Float u = std::fmod(point.x + 1., 2.) / 2.;
    Float v = std::fmod(1. - point.z, 2.) / 2.;
    return Vec2(u, v);
}

Vec2 CubeMap::cubeUVDown(Vec4 point)
{
    Float u = std::fmod(point.x + 1., 2.) / 2.;
    Float v = std::fmod(point.z + 1., 2.) / 2.;
    return Vec2(u, v);
}

Vec2 CubeMap::cubeUVFront(Vec4 point)
{
    Float u = std::fmod(point.x + 1., 2.) / 2.;
    Float v = std::fmod(point.y + 1., 2.) / 2.;
    return Vec2(u, v);
}

Vec2 CubeMap::cubeUVBack(Vec4 point)
{
    Float u = std::fmod(1. - point.x, 2.) / 2.;
    Float v = std::fmod(point.y + 1., 2.) / 2.;
    return Vec2(u, v);
}