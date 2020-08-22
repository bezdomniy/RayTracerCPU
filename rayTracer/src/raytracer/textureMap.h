#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

class TextureMap
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        // archive();
    }

public:
    TextureMap(/* args */);
    TextureMap(const TextureMap &textureMap);
    virtual ~TextureMap() = 0;
    virtual int faceFromPoint(glm::dvec4 point) = 0;

    virtual std::pair<glm::dvec2, int> uv_map(glm::dvec4 point) = 0;
};

class SphericalMap : public TextureMap
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<TextureMap>(this));
    }

public:
    SphericalMap(/* args */);
    ~SphericalMap();
    virtual int faceFromPoint(glm::dvec4 point) override;
    virtual std::pair<glm::dvec2, int> uv_map(glm::dvec4 point) override;
};

class PlanarMap : public TextureMap
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<TextureMap>(this));
    }

public:
    PlanarMap(/* args */);
    ~PlanarMap();
    virtual int faceFromPoint(glm::dvec4 point) override;
    virtual std::pair<glm::dvec2, int> uv_map(glm::dvec4 point) override;
};

class CylinderMap : public TextureMap
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<TextureMap>(this));
    }

public:
    CylinderMap(/* args */);
    ~CylinderMap();
    virtual int faceFromPoint(glm::dvec4 point) override;
    virtual std::pair<glm::dvec2, int> uv_map(glm::dvec4 point) override;
};

class CubeMap : public TextureMap
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<TextureMap>(this));
    }

public:
    CubeMap(/* args */);
    ~CubeMap();
    virtual std::pair<glm::dvec2, int> uv_map(glm::dvec4 point) override;
    virtual int faceFromPoint(glm::dvec4 point) override;

    glm::dvec2 cubeUVLeft(glm::dvec4 point);
    glm::dvec2 cubeUVRight(glm::dvec4 point);
    glm::dvec2 cubeUVUp(glm::dvec4 point);
    glm::dvec2 cubeUVDown(glm::dvec4 point);
    glm::dvec2 cubeUVFront(glm::dvec4 point);
    glm::dvec2 cubeUVBack(glm::dvec4 point);
};

// CEREAL_REGISTER_TYPE(TextureMap);
CEREAL_REGISTER_TYPE(SphericalMap);
CEREAL_REGISTER_TYPE(PlanarMap);
CEREAL_REGISTER_TYPE(CylinderMap);
CEREAL_REGISTER_TYPE(CubeMap);

CEREAL_REGISTER_POLYMORPHIC_RELATION(TextureMap, SphericalMap)
CEREAL_REGISTER_POLYMORPHIC_RELATION(TextureMap, PlanarMap)
CEREAL_REGISTER_POLYMORPHIC_RELATION(TextureMap, CylinderMap)
CEREAL_REGISTER_POLYMORPHIC_RELATION(TextureMap, CubeMap)