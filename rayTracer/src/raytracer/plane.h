#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>
// #include <vector>

class Plane : public Shape
{
public:
    Plane();
    ~Plane();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
    virtual std::pair<glm::dvec4, glm::dvec4> bounds() const override;
    virtual std::string type() override;

private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<Shape>(this));
    }
};

CEREAL_REGISTER_TYPE(Plane);