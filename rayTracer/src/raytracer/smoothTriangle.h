#pragma once

#include "triangle.h"

class SmoothTriangle : public Triangle
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::base_class<Triangle>(this), n1, n2, n3);
    }

    // template <class Archive>
    // static void load_and_construct(Archive &archive, cereal::construct<SmoothTriangle> &construct)
    // {
    //     glm::dvec3 p1;
    //     glm::dvec3 p2;
    //     glm::dvec3 p3;

    //     glm::dvec3 n1;
    //     glm::dvec3 n2;
    //     glm::dvec3 n3;

    //     archive(p1, p2, p3, n1, n2, n3);
    //     construct(p1, p2, p3, n1, n2, n3);
    // }

    glm::dvec3 n1;
    glm::dvec3 n2;
    glm::dvec3 n3;

public:
    SmoothTriangle() {}
    SmoothTriangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, glm::dvec3 n1, glm::dvec3 n2, glm::dvec3 n3);
    ~SmoothTriangle();

    // virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;

    // TODO make this match with other normalAts..
    virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
};

CEREAL_REGISTER_TYPE(SmoothTriangle);