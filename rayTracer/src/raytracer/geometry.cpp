#include "geometry.h"

glm::dvec4 Geometry::offset(const glm::dvec4 &p, const std::pair<glm::dvec4, glm::dvec4> &bounds)
{
    glm::dvec4 o = p - bounds.first;
    if (bounds.second.x > bounds.first.x)
        o.x /= bounds.second.x - bounds.first.x;
    if (bounds.second.y > bounds.first.y)
        o.y /= bounds.second.y - bounds.first.y;
    if (bounds.second.z > bounds.first.z)
        o.z /= bounds.second.z - bounds.first.z;
    return o;
}

glm::dvec4 Geometry::diagonal(const std::pair<glm::dvec4, glm::dvec4> &bounds) { return bounds.second - bounds.first; }

double Geometry::surfaceArea(const std::pair<glm::dvec4, glm::dvec4> &bounds)
{
    glm::dvec4 d = diagonal(bounds);
    return 2.0 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

uint32_t Geometry::nextPowerOfTwo(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

uint32_t Geometry::log2int(uint32_t val)
{
    if (val == 0)
        return std::numeric_limits<uint32_t>::max();
    if (val == 1)
        return 0;
    uint32_t ret = 0;
    while (val > 1)
    {
        val >>= 1;
        ret++;
    }
    return ret;
}