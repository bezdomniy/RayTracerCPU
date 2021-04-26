#include "group.h"

Group::Group(/* args */) : Shape()
{
  this->boundingBox = std::pair<Vec4, Vec4>(Vec4(std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), 1.), Vec4(-std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), 1.));
}

Group::~Group()
{
}

void Group::build(std::vector<std::shared_ptr<Shape>> &shapes, bool bvh)
{
  if (bvh)
  {
    auto start = std::chrono::high_resolution_clock::now();
    // TODO: can't use sharedfromthis in constructor, do differently
    recursiveBuild(shapes, 0, shapes.size());

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Time to build BVH: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;
  }
  else
  {
    this->boundingBox = std::pair<Vec4, Vec4>(Vec4(std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), 1.), Vec4(-std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), 1.));

    for (auto &shape : shapes)
    {
      // TODO update this so bounding box is only calculated when all shapes are added
      this->addChild(shape);
    }
  }
}

Group::Group(const Group &group)
{
  // TODO maybe make shapetype an enum instead of string
  for (const auto &child : group.children)
  {
    std::shared_ptr<Shape> nextShape;
    // std::shared_ptr<Material> mat = std::make_shared<Material>(*(child->material));

    if (child->type() == "Sphere")
    {
      std::shared_ptr<Sphere> spherePtr = std::dynamic_pointer_cast<Sphere>(child);
      nextShape = std::make_shared<Sphere>(*spherePtr);
    }
    else if (child->type() == "Plane")
    {
      std::shared_ptr<Plane> planePtr = std::dynamic_pointer_cast<Plane>(child);
      nextShape = std::make_shared<Plane>(*planePtr);
    }
    else if (child->type() == "Cube")
    {
      std::shared_ptr<Cube> cubePtr = std::dynamic_pointer_cast<Cube>(child);
      nextShape = std::make_shared<Cube>(*cubePtr);
    }
    else if (child->type() == "Cylinder")
    {
      std::shared_ptr<Cylinder> cylinderPtr = std::dynamic_pointer_cast<Cylinder>(child);
      nextShape = std::make_shared<Cylinder>(*cylinderPtr);
    }
    else if (child->type() == "Cone")
    {
      std::shared_ptr<Cone> conePtr = std::dynamic_pointer_cast<Cone>(child);
      nextShape = std::make_shared<Cone>(*conePtr);
    }
    else if (child->type() == "Triangle")
    {
      std::shared_ptr<Triangle> trianglePtr = std::dynamic_pointer_cast<Triangle>(child);
      nextShape = std::make_shared<Triangle>(*trianglePtr);
    }
    else if (child->type() == "Group")
    {
      std::shared_ptr<Group> groupPtr = std::dynamic_pointer_cast<Group>(child);
      nextShape = std::make_shared<Group>(*groupPtr);
    }
    else
    {
      throw std::runtime_error("unexpected shape type");
    }
    // else if (child->type() == "fir_branch")
    // {
    //   nextShape = std::make_shared<FirBranch>(*child);
    // }

    // nextShape->setMaterial(mat);

    nextShape->parent = child->parent;
    nextShape->material = child->material;
    // nextShape->materialSet = child->materialSet;
    // nextShape->transform = child->transform;
    nextShape->inverseTransform = child->inverseTransform;

    this->children.push_back(nextShape);
  }
  this->parent = group.parent;
  this->material = group.material;
  // this->materialSet = group.materialSet;
  // this->transform = group.transform;
  this->inverseTransform = group.inverseTransform;

  this->boundingBox = group.boundingBox;
}

void Group::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay;

  // if (!this->parent)
  transformedRay = transformRay(ray);
  // else
  //   transformedRay = ray;

  bool isHit = boundIntersection(transformedRay);

  if (isHit)
  {
    for (auto &shape : this->children)
    {
      // std::shared_ptr<Shape> p = shape.lock();
      shape->intersectRay(transformedRay, intersections);
    }
  }
  return;
}

Vec4 Group::normalAt(const Vec4 &point)
{
  throw std::runtime_error("group shouldn't call normal function");
  Vec4 objectPoint = this->inverseTransform * point;
  return glm::normalize(Vec4());
}

Vec4 Group::normalAt(const Vec4 &point, const Vec2 &uv)
{
  return normalAt(point);
}

void Group::setMaterial(std::shared_ptr<Material> &mat)
{
  this->material = mat;

  for (auto &child : this->children)
  {
    if (!child->material)
      child->setMaterial(mat);
  }
}

void Group::addChild(std::shared_ptr<Shape> &child)
{
  child->parent = shared_from_this();

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
  else if (child->type() == "Model")
  {
    this->children.push_back(std::dynamic_pointer_cast<Model>(child));
  }
  else
  {
    throw std::invalid_argument(child->type() + " is not a valid shape for group");
  }

  updateBoundingBox(child);
}

void Group::updateBoundingBox(std::shared_ptr<Shape> &shape)
{
  std::vector<Vec4> points(8);
  const auto &bounds = shape->bounds();
  points.at(0) = bounds.first;
  points.at(1) = Vec4(bounds.first.x, bounds.first.y, bounds.second.z, 1.);
  points.at(2) = Vec4(bounds.first.x, bounds.second.y, bounds.first.z, 1.);
  points.at(3) = Vec4(bounds.first.x, bounds.second.y, bounds.second.z, 1.);
  points.at(4) = Vec4(bounds.second.x, bounds.first.y, bounds.first.z, 1.);
  points.at(5) = Vec4(bounds.second.x, bounds.first.y, bounds.second.z, 1.);
  points.at(6) = Vec4(bounds.second.x, bounds.second.y, bounds.first.z, 1.);
  points.at(7) = bounds.second;

  Mat4 transform = glm::affineInverse(shape->inverseTransform);

  for (auto point : points)
  {
    Vec4 transformedPoint(transform * point);
    this->boundingBox.first = glm::min(this->boundingBox.first, transformedPoint);
    this->boundingBox.second = glm::max(this->boundingBox.second, transformedPoint);
  }
}

std::pair<Vec4, Vec4> Group::bounds() const
{
  return this->boundingBox;
}

bool Group::boundIntersection(Ray &transformedRay)
{
  Float t_min = -std::numeric_limits<Float>::infinity();
  Float t_max = std::numeric_limits<Float>::infinity();

  for (int a = 0; a < 3; a++)
  {
    auto invD = 1.0 / transformedRay.direction[a];
    auto t0 = (this->boundingBox.first[a] - transformedRay.origin[a]) * invD;
    auto t1 = (this->boundingBox.second[a] - transformedRay.origin[a]) * invD;
    if (invD < 0.0)
      std::swap(t0, t1);
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
      return false;
  }
  return true;

  // std::pair<double, double> xtminmax = Geometry::checkAxis<Float>(transformedRay.origin.x, transformedRay.direction.x, this->boundingBox.first.x, this->boundingBox.second.x);
  // std::pair<double, double> ytminmax = Geometry::checkAxis<Float>(transformedRay.origin.y, transformedRay.direction.y, this->boundingBox.first.y, this->boundingBox.second.y);
  // std::pair<double, double> ztminmax = Geometry::checkAxis<Float>(transformedRay.origin.z, transformedRay.direction.z, this->boundingBox.first.z, this->boundingBox.second.z);

  // Float tmin = std::max({xtminmax.first, ytminmax.first, ztminmax.first});
  // Float tmax = std::min({xtminmax.second, ytminmax.second, ztminmax.second});

  // return !(tmin > tmax);
}

std::pair<Vec4, Vec4> Group::mergeBounds(const std::pair<Vec4, Vec4> b1, const std::pair<Vec4, Vec4> b2)
{
  return std::pair<Vec4, Vec4>(Vec4(std::min(b1.first.x, b2.first.x),
                                    std::min(b1.first.y, b2.first.y),
                                    std::min(b1.first.z, b2.first.z), 1.),
                               Vec4(std::max(b1.second.x, b2.second.x),
                                    std::max(b1.second.y, b2.second.y),
                                    std::max(b1.second.z, b2.second.z), 1.));
}

std::pair<Vec4, Vec4> Group::mergeBounds(const std::pair<Vec4, Vec4> b1, const Vec4 p)
{
  return std::pair<Vec4, Vec4>(Vec4(std::min(b1.first.x, p.x),
                                    std::min(b1.first.y, p.y),
                                    std::min(b1.first.z, p.z), 1.),
                               Vec4(std::max(b1.second.x, p.x),
                                    std::max(b1.second.y, p.y),
                                    std::max(b1.second.z, p.z), 1.));
}

std::shared_ptr<Group> Group::recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end)
{
  std::shared_ptr<Group> node;
  if (start == 0 && end == shapes.size())
  {
    node = shared_from_this();
  }
  else
  {
    node = std::make_shared<Group>();
  }

  int nShapes = end - start;

  if (nShapes == 1)
  {
    // for (int i = start; i < end; ++i)
    // 	node->addChild(shapes.at(i));

    node->addChild(shapes.at(start));

    //        if (nShapes == 2)
    //            node->addChild(shapes.at(start + 1));
    return node;
  }
  else
  {
    // std::pair<Vec4, Vec4> centroidBounds;
    std::pair<Vec4, Vec4> centroidBounds{
        Vec4(std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), 1.f),
        Vec4(-std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), 1.f)};

    //for (const auto &shape : shapes)
    for (auto it = shapes.begin() + start; it != shapes.begin() + end; ++it)
      centroidBounds = mergeBounds(centroidBounds, (*it)->boundsCentroid());

    Vec4 diagonal = centroidBounds.second - centroidBounds.first;
    int splitDimension;

    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
      splitDimension = 0;
    else if (diagonal.y > diagonal.z)
      splitDimension = 1;
    else
      splitDimension = 2;

    int mid = (start + end) / 2;

    if (centroidBounds.first[splitDimension] == centroidBounds.second[splitDimension])
    {
      for (int i = start; i < end; ++i)
        node->addChild(shapes.at(i));
      return node;
    }
    else
    {
      if (false)
      {
        mid = (start + end) / 2;
        std::nth_element(&shapes[start], &shapes[mid],
                         &shapes[end - 1] + 1,
                         [splitDimension](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) {
                           return a->boundsCentroid()[splitDimension] < b->boundsCentroid()[splitDimension];
                         });
      }
      else
      {
        // Partition primitives using approximate SAH
        if (nShapes <= 2)
        {
          mid = (start + end) / 2;
          std::nth_element(&shapes[start], &shapes[mid],
                           &shapes[end - 1] + 1,
                           [splitDimension](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) {
                             return a->boundsCentroid()[splitDimension] < b->boundsCentroid()[splitDimension];
                           });
        }
        else
        {
          // Allocate _BucketInfo_ for SAH partition buckets
          constexpr int nBuckets = 12;
          constexpr int maxPrimsInNode = 4;
          Geometry::BucketInfo buckets[nBuckets];

          // Initialize _BucketInfo_ for SAH partition buckets
          for (int i = start; i < end; ++i)
          {
            int b = nBuckets * Geometry::offset(shapes[i]->boundsCentroid(), centroidBounds)[splitDimension];
            if (b == nBuckets)
              b = nBuckets - 1;

            buckets[b].count++;
            buckets[b].bounds =
                mergeBounds(buckets[b].bounds, shapes[i]->bounds());
          }

          // Compute costs for splitting after each bucket
          Float cost[nBuckets - 1];
          for (int i = 0; i < nBuckets - 1; ++i)
          {
            std::pair<Vec4, Vec4> b0, b1;
            int count0 = 0, count1 = 0;
            for (int j = 0; j <= i; ++j)
            {
              b0 = mergeBounds(b0, buckets[j].bounds);
              count0 += buckets[j].count;
            }
            for (int j = i + 1; j < nBuckets; ++j)
            {
              b1 = mergeBounds(b1, buckets[j].bounds);
              count1 += buckets[j].count;
            }

            std::pair<Vec4, Vec4> bounds{
                Vec4(std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), std::numeric_limits<Float>::infinity(), 1.f),
                Vec4(-std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), -std::numeric_limits<Float>::infinity(), 1.f)};

            for (auto it = shapes.begin() + start; it != shapes.begin() + end; ++it)
              bounds = mergeBounds(bounds, (*it)->bounds());

            cost[i] = 1 +
                      (count0 * Geometry::surfaceArea(b0) +
                       count1 * Geometry::surfaceArea(b1)) /
                          Geometry::surfaceArea(bounds);
          }

          // Find bucket to split at that minimizes SAH metric
          Float minCost = cost[0];
          int minCostSplitBucket = 0;
          for (int i = 1; i < nBuckets - 1; ++i)
          {
            if (cost[i] < minCost)
            {
              minCost = cost[i];
              minCostSplitBucket = i;
            }
          }

          // Either create leaf or split primitives at selected SAH
          // bucket
          float leafCost = nShapes;
          if (nShapes > maxPrimsInNode || minCost < leafCost)
          {
            auto pmid = std::partition(
                &shapes[start], &shapes[end - 1] + 1,
                [=](const std::shared_ptr<Shape> &pi) {
                  int b = nBuckets *
                          Geometry::offset(pi->boundsCentroid(), centroidBounds)[splitDimension];
                  if (b == nBuckets)
                    b = nBuckets - 1;
                  return b <= minCostSplitBucket;
                });
            mid = pmid - &shapes[0];
          }
          else
          {
            // Create leaf
            for (int i = start; i < end; ++i)
            {
              node->addChild(shapes.at(i));
            }

            return node;
          }
        }
      }

      //            if (start !=mid)
      std::shared_ptr<Shape> leftChild = recursiveBuild(shapes, start, mid);

      //            if (end !=mid)
      std::shared_ptr<Shape> rightChild = recursiveBuild(shapes, mid, end);

      node->addChild(leftChild);
      node->addChild(rightChild);
    }
  }

  return node;
}

std::string Group::type() { return "Group"; }
