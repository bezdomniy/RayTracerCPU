#pragma once
#include "types.h"
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
    virtual int faceFromPoint(Vec4 point) = 0;

    virtual std::pair<Vec2, int> uv_map(Vec4 point) = 0;
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
    virtual int faceFromPoint(Vec4 point) override;
    virtual std::pair<Vec2, int> uv_map(Vec4 point) override;
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
    virtual int faceFromPoint(Vec4 point) override;
    virtual std::pair<Vec2, int> uv_map(Vec4 point) override;
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
    virtual int faceFromPoint(Vec4 point) override;
    virtual std::pair<Vec2, int> uv_map(Vec4 point) override;
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
    virtual std::pair<Vec2, int> uv_map(Vec4 point) override;
    virtual int faceFromPoint(Vec4 point) override;

    Vec2 cubeUVLeft(Vec4 point);
    Vec2 cubeUVRight(Vec4 point);
    Vec2 cubeUVUp(Vec4 point);
    Vec2 cubeUVDown(Vec4 point);
    Vec2 cubeUVFront(Vec4 point);
    Vec2 cubeUVBack(Vec4 point);
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