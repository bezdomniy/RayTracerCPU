#pragma once

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>

// TODO try to define load_and_construct for none-default constructor types here

namespace glm
{
    template <typename Archive>
    void serialize(Archive &archive, glm::dvec2 &v2)
    {
        archive(v2.x, v2.y);
    }
    template <typename Archive>
    void serialize(Archive &archive, glm::dvec3 &v3)
    {
        archive(v3.x, v3.y, v3.z);
    }
    template <typename Archive>
    void serialize(Archive &archive, glm::dvec4 &v4)
    {
        archive(v4.x, v4.y, v4.z, v4.w);
    }
    template <typename Archive>
    void serialize(Archive &archive, glm::dmat4 &m4)
    {
        archive(m4[0], m4[1], m4[2], m4[3]);
    }
} // namespace glm