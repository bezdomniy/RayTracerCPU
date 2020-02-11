#pragma once

#include "shape.h"
#include "sphere.h"
#include "cube.h"
#include "plane.h"
#include "triangle.h"
#include "cylinder.h"


#include <vector>
#include <iostream>

class Group : public Shape, public std::enable_shared_from_this<Group>
{
private:
    /* data */
public:
    Group(/* args */);
    ~Group();

    std::vector<std::weak_ptr<Shape>> children;

    void addChild(std::shared_ptr<Shape>& child);

    virtual void intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) override;
    virtual glm::dvec4 normalAt(glm::dvec4 point) override ;
    virtual std::string type() override;
};

