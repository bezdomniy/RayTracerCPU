#pragma once

#include "shape.h"
#include "sphere.h"
#include "cube.h"
#include "plane.h"
#include "triangle.h"
#include "smoothTriangle.h"
#include "cylinder.h"
#include "cone.h"
#include "model.h"

#include <vector>
#include <memory>

#include <cereal/types/utility.hpp>

class Group : public Shape, public std::enable_shared_from_this<Group>
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<Shape>(this), children, boundingBox);
    }

    bool boundIntersection(Ray &transformedRay);

    std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4> b1, const std::pair<Vec4, Vec4> b2);
    std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4> b1, const Vec4 p);
    std::shared_ptr<Group> recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end);

public:
    Group();
    void build(std::vector<std::shared_ptr<Shape>> &shapes, bool bvh);
    Group(const Group &group);
    ~Group();

    std::vector<std::shared_ptr<Shape>> children;
    std::pair<Vec4, Vec4> boundingBox;

    void updateBoundingBox(std::shared_ptr<Shape> &shape);

    void addChild(std::shared_ptr<Shape> &child);
    void setMaterial(std::shared_ptr<Material> &mat) override;

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual Vec4 normalAt(const Vec4 &point) override;
    virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
    virtual std::pair<Vec4, Vec4> bounds() const override;
    virtual std::string type() override;
};

CEREAL_REGISTER_TYPE(Group);