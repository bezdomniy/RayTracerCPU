#pragma once

#include "shape.h"
#include "geometry.h"
#include "types.h"

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
    Triangle(Vec3 p1, Vec3 p2, Vec3 p3);
    ~Triangle();

    //    TODO: change this to a precalculated value in all and remove the function call
    std::pair<Vec4, Vec4> boundsv;

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual Vec4 normalAt(const Vec4 &point) override;
    virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
    virtual std::pair<Vec4, Vec4> bounds() const final override;
    virtual std::string type() override;

protected:
    // TODO: merge normal and n1 in smooth triangle to save space
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    Vec3 n1;
    // // TODO: remove e1 and e2
    // Vec3 e1;
    // Vec3 e2;
};

CEREAL_REGISTER_TYPE(Triangle);
