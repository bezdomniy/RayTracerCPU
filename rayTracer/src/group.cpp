#include "group.h"

Group::Group(/* args */) : Shape()
{
  this->boundingBox = std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.), glm::dvec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.));
}

Group::~Group()
{
}

Group::Group(std::vector<std::shared_ptr<Shape>> &shapes) : Shape()
{
  this->boundingBox = std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.), glm::dvec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.));

  for (auto &shape : shapes)
  {
    // TODO update this so bounding box is only calculated when all shapes are added
    this->addChild(shape);
  }
}

void Group::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);

  if (boundIntersection(transformedRay))
  {
    for (auto &shape : this->children)
    {
      // std::shared_ptr<Shape> p = shape.lock();
      shape->intersectRay(transformedRay, intersections);
    }
  }
  return;
}

glm::dvec4 Group::normalAt(glm::dvec4 point)
{
  throw std::runtime_error("group shouldn't call normal function");
  glm::dvec4 objectPoint = this->inverseTransform * point;
  return glm::normalize(glm::dvec4());
}

glm::dvec4 Group::normalAt(glm::dvec4 point, glm::dvec2 uv)
{
  return normalAt(point);
}

void Group::setMaterial(std::shared_ptr<Material> &mat)
{
  this->material = mat;

  for (auto &child : this->children)
  {
    if (!child->materialSet)
      child->setMaterial(mat);
  }
}

void Group::addChild(std::shared_ptr<Shape> &child)
{
  child->parent = this;

  if (this->material && !child->material)
    child->setMaterial(this->material);

  if (child->type() == "Sphere")
  {
    this->children.push_back(std::dynamic_pointer_cast<Sphere>(child));
  }
  else if (child->type() == "Plane")
  {
    this->children.push_back(std::dynamic_pointer_cast<Plane>(child));
  }
  else if (child->type() == "Cube")
  {
    this->children.push_back(std::dynamic_pointer_cast<Cube>(child));
  }
  else if (child->type() == "Cylinder")
  {
    this->children.push_back(std::dynamic_pointer_cast<Cylinder>(child));
  }
  else if (child->type() == "Triangle")
  {
    this->children.push_back(std::dynamic_pointer_cast<Triangle>(child));
  }
  else if (child->type() == "Group")
  {
    this->children.push_back(std::dynamic_pointer_cast<Group>(child));
  }
  else if (child->type() == "Cone")
  {
    this->children.push_back(std::dynamic_pointer_cast<Cone>(child));
  }
  else
  {
    throw std::invalid_argument(child->type() + " is not a valid shape for group");
  }

  updateBoundingBox(child);
}

void Group::updateBoundingBox(std::shared_ptr<Shape> &shape)
{
  std::vector<glm::dvec4> points(8);
  points.at(0) = shape->bounds().first;
  points.at(1) = glm::dvec4(shape->bounds().first.x, shape->bounds().first.y, shape->bounds().second.z, 1.);
  points.at(2) = glm::dvec4(shape->bounds().first.x, shape->bounds().second.y, shape->bounds().first.z, 1.);
  points.at(3) = glm::dvec4(shape->bounds().first.x, shape->bounds().second.y, shape->bounds().second.z, 1.);
  points.at(4) = glm::dvec4(shape->bounds().second.x, shape->bounds().first.y, shape->bounds().first.z, 1.);
  points.at(5) = glm::dvec4(shape->bounds().second.x, shape->bounds().first.y, shape->bounds().second.z, 1.);
  points.at(6) = glm::dvec4(shape->bounds().second.x, shape->bounds().second.y, shape->bounds().first.z, 1.);
  points.at(7) = shape->bounds().second;

  for (auto point : points)
  {
    glm::dvec4 transformedPoint(shape->transform * point);
    this->boundingBox.first = glm::min(this->boundingBox.first, transformedPoint);
    this->boundingBox.second = glm::max(this->boundingBox.second, transformedPoint);
  }
}

std::pair<glm::dvec4, glm::dvec4> Group::bounds()
{
  return this->boundingBox;
}

bool Group::boundIntersection(Ray &transformedRay)
{
  std::pair<double, double> xtminmax = Geometry::checkAxis<double>(transformedRay.origin.x, transformedRay.direction.x, this->boundingBox.first.x, this->boundingBox.second.x);
  std::pair<double, double> ytminmax = Geometry::checkAxis<double>(transformedRay.origin.y, transformedRay.direction.y, this->boundingBox.first.y, this->boundingBox.second.y);
  std::pair<double, double> ztminmax = Geometry::checkAxis<double>(transformedRay.origin.z, transformedRay.direction.z, this->boundingBox.first.z, this->boundingBox.second.z);

  double tmin = std::max({xtminmax.first, ytminmax.first, ztminmax.first});
  double tmax = std::min({xtminmax.second, ytminmax.second, ztminmax.second});

  if (tmin > tmax)
    return false;

  return true;
}

std::string Group::type() { return "Group"; }