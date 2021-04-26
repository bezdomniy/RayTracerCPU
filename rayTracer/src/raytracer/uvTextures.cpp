#include "uvTextures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

UVTexture::UVTexture(/* args */)
{
}

UVTexture::~UVTexture()
{
}

CheckeredTexture::CheckeredTexture()
{
}

CheckeredTexture::CheckeredTexture(Vec3 colourA, Vec3 colourB, int width, int height)
{

    this->colourA = colourA;
    this->colourB = colourB;
    this->width = width;
    this->height = height;
}

CheckeredTexture::~CheckeredTexture()
{
}

Vec3 CheckeredTexture::patternAt(Vec2 uv, int faceIndex)
{
    int u2 = (int)std::floor(uv.x * this->width);
    int v2 = (int)std::floor(uv.y * this->height);

    if ((u2 + v2) % 2 == 0)
    {
        return this->colourA;
    }
    else
    {
        return this->colourB;
    }
}

void CheckeredTexture::loadRight(std::string const &path)
{
}

void CheckeredTexture::loadLeft(std::string const &path)
{
}

void CheckeredTexture::loadUp(std::string const &path)
{
}

void CheckeredTexture::loadDown(std::string const &path)
{
}

void CheckeredTexture::loadFront(std::string const &path)
{
}

void CheckeredTexture::loadBack(std::string const &path)
{
}

ImageTexture::ImageTexture()
{
    this->textures.resize(6);
}

// #include <iostream>
// #include <filesystem>
ImageTexture::ImageTexture(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.push_back(std::move(surface));
}

ImageTexture::~ImageTexture()
{
    // for (auto &surface : this->textures)
    // {
    //     // stbi_image_free(&surface->rgb[0]);
    //     // printf("freeing texture.");

    //     stbi_image_free(surface->rgb.get());
    //     // std::cout << "freed texture." << std::endl;
    // }
}

Vec3 ImageTexture::rgbFromSurface(std::unique_ptr<Surface> &surface, int x, int y)
{
    // unsigned char *p = (uint8_t *)surface->rgb + y * surface->w * surface->bpp + x * surface->bpp;
    unsigned char *p = (uint8_t *)surface->rgb.data() + y * surface->w * surface->bpp + x * surface->bpp;

    // if (std::endian::native == std::endian::big)
    int n = 1;
    // Check little endian
    if (*(char *)&n == 1)
    {
        Float r = (p[0] / 255.);
        Float g = (p[1] / 255.);
        Float b = (p[2] / 255.);

        return Vec3(r, g, b);
    }

    else
    {
        Float r = (p[2] / 255.);
        Float g = (p[1] / 255.);
        Float b = (p[0] / 255.);

        return Vec3(r, g, b);
    }
}

Vec3 ImageTexture::patternAt(Vec2 uv, int faceIndex)
{
    Float u = uv.x;
    Float v = 1. - uv.y;

    Float x = u * (this->textures.at(faceIndex)->w - 1);
    Float y = v * (this->textures.at(faceIndex)->h - 1);

    return rgbFromSurface(this->textures.at(faceIndex), (int)std::round(x), (int)std::round(y));
}

// TODO fix duplication in these
void ImageTexture::loadRight(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(0) = std::move(surface);
}

void ImageTexture::loadLeft(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(1) = std::move(surface);
}

void ImageTexture::loadUp(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(2) = std::move(surface);
}

void ImageTexture::loadDown(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(3) = std::move(surface);
}

void ImageTexture::loadFront(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(4) = std::move(surface);
}

void ImageTexture::loadBack(std::string const &path)
{
    int w, h, bpp;
    std::unique_ptr<Surface> surface = std::make_unique<Surface>(w, h, bpp);

    // surface->rgb = std::unique_ptr<unsigned char>(stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb));
    unsigned char *rgb_temp = stbi_load(path.c_str(), &surface->w, &surface->h, &surface->bpp, STBI_rgb);
    if (!rgb_temp)
    {
        // std::cout << path << std::endl;
        throw std::invalid_argument("can't find file: " + path);
    }

    surface->rgb = std::vector<unsigned char>(rgb_temp, rgb_temp + (surface->w * surface->h * STBI_rgb * sizeof(unsigned char)));
    stbi_image_free(rgb_temp);
    this->textures.at(5) = std::move(surface);
}