#include "textureMap.h"

TextureMap::TextureMap(/* args */)
{
}

TextureMap::TextureMap(const TextureMap &textureMap) {

}

TextureMap::~TextureMap()
{
}

SphericalMap::SphericalMap(/* args */)
{
}

SphericalMap::~SphericalMap()
{
}

glm::dvec2 SphericalMap::uv_map(glm::dvec4 point) {
    double theta = glm::atan(point.x, point.z);

    double radius = glm::length(glm::dvec3(point));

    double phi = glm::acos(point.y / radius);

    double raw_u = theta / (2 * glm::pi<double>());
    double u = 1.0 - (raw_u + 0.5);

    double v = 1.0 - phi / glm::pi<double>();

    return glm::dvec2(u,v);
}
