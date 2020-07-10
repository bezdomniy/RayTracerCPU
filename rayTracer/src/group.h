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

class Group : public Shape, std::enable_shared_from_this<Group>
{

private:
    /* data */

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
    virtual glm::dvec4 normalAt(glm::dvec4 point) override;
    virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
    virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
    virtual std::string type() override;
};
