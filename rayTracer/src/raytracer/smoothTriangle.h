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
    //     Vec3 p1;
    //     Vec3 p2;
    //     Vec3 p3;

    //     Vec3 n1;
    //     Vec3 n2;
    //     Vec3 n3;

    //     archive(p1, p2, p3, n1, n2, n3);
    //     construct(p1, p2, p3, n1, n2, n3);
    // }

    Vec3 n1;
    Vec3 n2;
    Vec3 n3;

public:
    SmoothTriangle() {}
    SmoothTriangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 n1, Vec3 n2, Vec3 n3);
    ~SmoothTriangle();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;

    // TODO make this match with other normalAts..
    virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
};

CEREAL_REGISTER_TYPE(SmoothTriangle);