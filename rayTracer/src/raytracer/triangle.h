#pragma once

#include "shape.h"
#include "geometry.h"
#include "types.h"

class Triangle : public Shape
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<Shape>(this), p1, p2, p3, e1, e2, normal);
    }

    // template <class Archive>
    // static void load_and_construct(Archive &archive, cereal::construct<Triangle> &construct)
    // {
    //     Vec3 p1;
    //     Vec3 p2;
    //     Vec3 p3;

    //     archive(p1, p2, p3);
    //     construct(p1, p2, p3);
    // }

public:
    Triangle() {}
    Triangle(Vec3 p1, Vec3 p2, Vec3 p3);
    ~Triangle();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual Vec4 normalAt(const Vec4 &point) override;
    virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
    virtual std::pair<Vec4, Vec4> bounds() override;
    virtual std::string type() override;

protected:
    Vec4 normal;
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    Vec3 e1;
    Vec3 e2;
};

CEREAL_REGISTER_TYPE(Triangle);