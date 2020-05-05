#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class TextureMap
{
private:
    /* data */
public:
    TextureMap(/* args */);
    TextureMap(const TextureMap &textureMap);
    virtual ~TextureMap();

    virtual glm::dvec2 uv_map(glm::dvec4 point) = 0;
};

class SphericalMap: public TextureMap
{
private:
    /* data */
public:
    SphericalMap(/* args */);
    ~SphericalMap();
    virtual glm::dvec2 uv_map(glm::dvec4 point) override;
};

