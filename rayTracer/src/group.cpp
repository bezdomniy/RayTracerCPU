#include "group.h"

Group::Group(/* args */) : Shape()
{
}

Group::~Group()
{
}

// TODO
void Group::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) { 
    Ray transformedRay = transformRay(ray);

}

glm::dvec4 Group::normalAt(glm::dvec4 point) {
  std::cout << "Shouldn't be called.\n";
  glm::dvec4 objectPoint = this->inverseTransform * point;
  return glm::normalize(glm::dvec4());
}

void Group::addChild(std::shared_ptr<Shape>& child) {
  child->parent = shared_from_this();

if (child->type() == "Sphere") {
    this->children.push_back(std::dynamic_pointer_cast<Sphere>(child));
  } else if (child->type() == "Plane") {
    this->children.push_back(std::dynamic_pointer_cast<Plane>(child));
  } else if (child->type() == "Cube") {
    this->children.push_back(std::dynamic_pointer_cast<Cube>(child));
  } else if (child->type() == "Cylinder") {
    this->children.push_back(std::dynamic_pointer_cast<Cylinder>(child));
  } else if (child->type() == "Triangle") {
    this->children.push_back(std::dynamic_pointer_cast<Triangle>(child));
  } else {
    throw std::invalid_argument("Not a valid shape for group");
  }
}


std::string Group::type() { return "Group"; }