#pragma once

#include <glm/glm.hpp>

#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include "serialisation.h"
// #include <bit>

class UVTexture
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        // archive();
    }

public:
    UVTexture(/* args */);
    ~UVTexture();

    virtual void loadRight(std::string const &path) = 0;
    virtual void loadLeft(std::string const &path) = 0;
    virtual void loadUp(std::string const &path) = 0;
    virtual void loadDown(std::string const &path) = 0;
    virtual void loadFront(std::string const &path) = 0;
    virtual void loadBack(std::string const &path) = 0;

    virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) = 0;
};

class CheckeredTexture : public UVTexture
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::base_class<UVTexture>(this), colourA, colourB, width, height);
    }

    virtual void loadRight(std::string const &path) override;
    virtual void loadLeft(std::string const &path) override;
    virtual void loadUp(std::string const &path) override;
    virtual void loadDown(std::string const &path) override;
    virtual void loadFront(std::string const &path) override;
    virtual void loadBack(std::string const &path) override;

public:
    CheckeredTexture();
    CheckeredTexture(glm::dvec3 colourA, glm::dvec3 colourB, int width, int height);
    ~CheckeredTexture();
    virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) override;

    glm::dvec3 colourA;
    glm::dvec3 colourB;
    int width;
    int height;
};

class ImageTexture : public UVTexture
{
private:
    struct Surface
    {
    private:
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive &archive)
        {
            archive(rgb, w, h, bpp);
        }

        template <class Archive>
        static void load_and_construct(Archive &archive, cereal::construct<Surface> &construct)
        {
            int w;
            int h;
            int bpp;

            archive(w, h, bpp);
            construct(w, h, bpp);
        }

    public:
        Surface(int w, int h, int bpp)
        {
            this->w = w;
            this->h = h;
            this->bpp = bpp;
        }

        std::unique_ptr<unsigned char> rgb;
        int w;
        int h;
        int bpp;
    };

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::base_class<UVTexture>(this), textures, width, height);
    }

    glm::dvec3 rgbFromSurface(std::unique_ptr<Surface> &surface, int x, int y);

public:
    ImageTexture();
    ImageTexture(std::string const &path);
    ~ImageTexture();

    virtual void loadRight(std::string const &path) override;
    virtual void loadLeft(std::string const &path) override;
    virtual void loadUp(std::string const &path) override;
    virtual void loadDown(std::string const &path) override;
    virtual void loadFront(std::string const &path) override;
    virtual void loadBack(std::string const &path) override;

    virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) override;

    std::vector<std::unique_ptr<Surface>> textures;
    int width;
    int height;
};

CEREAL_REGISTER_TYPE(CheckeredTexture);
CEREAL_REGISTER_TYPE(ImageTexture);