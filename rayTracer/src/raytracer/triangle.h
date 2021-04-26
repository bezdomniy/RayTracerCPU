#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>

#include "cereal/types/utility.hpp"

class Triangle : public Shape
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<Shape>(this), p1, p2, p3, n1, boundsv);
    }

public:
    Triangle() {}
    Triangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3);
    ~Triangle();

    //    TODO: change this to a precalculated value in all and remove the function call
    std::pair<glm::dvec4, glm::dvec4> boundsv;

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
    virtual std::pair<glm::dvec4, glm::dvec4> bounds() const final override;
    virtual std::string type() override;

protected:
    // TODO: merge normal and n1 in smooth triangle to save space
    glm::dvec3 p1;
    glm::dvec3 p2;
    glm::dvec3 p3;
    glm::dvec3 n1;
    // // TODO: remove e1 and e2
    // glm::dvec3 e1;
    // glm::dvec3 e2;
};

CEREAL_REGISTER_TYPE(Triangle);
