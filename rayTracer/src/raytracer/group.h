#pragma once

#include "shape.h"
#include "sphere.h"
#include "cube.h"
#include "plane.h"
#include "triangle.h"
#include "smoothTriangle.h"
#include "cylinder.h"
#include "cone.h"

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
    std::shared_ptr<Group> getptr();

public:
    Group();
    Group(std::vector<std::shared_ptr<Shape>> &shapes);
    Group(const Group &group);
    ~Group();

    std::vector<std::shared_ptr<Shape>> children;
    std::pair<glm::dvec4, glm::dvec4> boundingBox;

    void updateBoundingBox(std::shared_ptr<Shape> &shape);

    void addChild(std::shared_ptr<Shape> &child);
    void setMaterial(std::shared_ptr<Material> &mat) override;

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point) override;
    virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
    virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
    virtual std::string type() override;
};

CEREAL_REGISTER_TYPE(Group);