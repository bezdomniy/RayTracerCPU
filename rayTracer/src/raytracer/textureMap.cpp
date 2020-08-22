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

int SphericalMap::faceFromPoint(glm::dvec4 point)
{
    return 0;
}

std::pair<glm::dvec2, int> SphericalMap::uv_map(glm::dvec4 point)
{
    double theta = glm::atan(point.x, point.z);

    double radius = glm::length(glm::dvec3(point));

    double phi = glm::acos(point.y / radius);

    double raw_u = theta / (2 * glm::pi<double>());
    double u = 1.0 - (raw_u + 0.5);

    double v = 1.0 - phi / glm::pi<double>();

    return {glm::dvec2(u, v), 0};
}

PlanarMap::PlanarMap()
{
}

PlanarMap::~PlanarMap()
{
}

int PlanarMap::faceFromPoint(glm::dvec4 point)
{
    return 0;
}

std::pair<glm::dvec2, int> PlanarMap::uv_map(glm::dvec4 point)
{
    double u = std::fmod(point.x, 1);
    double v = std::fmod(point.z, 1);
    return {glm::dvec2(u, v), 0};
}

CylinderMap::CylinderMap()
{
}

CylinderMap::~CylinderMap()
{
}

int CylinderMap::faceFromPoint(glm::dvec4 point)
{
    // TODO update to get caps
    return 0;
}

// #include <iostream>
std::pair<glm::dvec2, int> CylinderMap::uv_map(glm::dvec4 point)
{
    double theta = glm::atan(point.x, point.z);
    double raw_u = theta / (2 * glm::pi<double>());
    double u = 1.0 - (raw_u + 0.5);

    double v = std::fmod(point.y, 1);

    // std::cout << u << " | " << v << std::endl;
    // std::cout << point.x << " " << point.y << " " << point.z << " " << std::endl;

    return {glm::dvec2(u, v), 0};
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

std::pair<glm::dvec2, int> CubeMap::uv_map(glm::dvec4 point)
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
    else if (face == 5)
    {
        return {cubeUVBack(point), face};
    }
}

int CubeMap::faceFromPoint(glm::dvec4 point)
{
    std::vector<double> coords = {point.x, point.y, point.z};
    std::vector<double> absCoords = {std::abs(point.x), std::abs(point.y), std::abs(point.z)};

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

glm::dvec2 CubeMap::cubeUVLeft(glm::dvec4 point)
{
    double u = std::fmod(point.z + 1., 2.) / 2.;
    double v = std::fmod(point.y + 1., 2.) / 2.;
    return glm::dvec2(u, v);
}

glm::dvec2 CubeMap::cubeUVRight(glm::dvec4 point)
{
    double u = std::fmod(1. - point.z, 2.) / 2.;
    double v = std::fmod(point.y + 1., 2.) / 2.;
    return glm::dvec2(u, v);
}

glm::dvec2 CubeMap::cubeUVUp(glm::dvec4 point)
{
    double u = std::fmod(point.x + 1., 2.) / 2.;
    double v = std::fmod(1. - point.z, 2.) / 2.;
    return glm::dvec2(u, v);
}

glm::dvec2 CubeMap::cubeUVDown(glm::dvec4 point)
{
    double u = std::fmod(point.x + 1., 2.) / 2.;
    double v = std::fmod(point.z + 1., 2.) / 2.;
    return glm::dvec2(u, v);
}

glm::dvec2 CubeMap::cubeUVFront(glm::dvec4 point)
{
    double u = std::fmod(point.x + 1., 2.) / 2.;
    double v = std::fmod(point.y + 1., 2.) / 2.;
    return glm::dvec2(u, v);
}

glm::dvec2 CubeMap::cubeUVBack(glm::dvec4 point)
{
    double u = std::fmod(1. - point.x, 2.) / 2.;
    double v = std::fmod(point.y + 1., 2.) / 2.;
    return glm::dvec2(u, v);
}