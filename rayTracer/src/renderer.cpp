#include "renderer.h"

Renderer::Renderer() {}

Renderer::Renderer(std::shared_ptr<Camera> &c)
{
  this->camera = c;
  this->canvas = Canvas(this->camera->hsize, this->camera->vsize);
}

Renderer::~Renderer() {}

void Renderer::render(World &world)
{
  int sqrtRaysPerPixel = std::sqrt(RAYS_PER_PIXEL);
  float halfSubPixelSize = 1.f / (float)sqrtRaysPerPixel / 2.f;

  std::vector<std::pair<int, int>> pixels;
  pixels.reserve(this->canvas.height * this->canvas.width);

  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      pixels.push_back(std::make_pair(x, y));
    }
  }

  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(pixels.begin(), pixels.end(), g);

  tf::Executor executor(std::thread::hardware_concurrency());
  tf::Taskflow taskflow;

  taskflow.parallel_for(
      pixels.begin(), pixels.end(),
      [this, &world, sqrtRaysPerPixel, halfSubPixelSize](auto &pixel) {
        renderPixel(world, pixel, sqrtRaysPerPixel, halfSubPixelSize);
      });
  executor.run(taskflow);
  executor.wait_for_all();
  // for (std::vector<std::pair<int, int>>::iterator it = pixels.begin();
  //      it != pixels.end(); ++it)
  // {
  //   renderPixel(world, *it, sqrtRaysPerPixel, halfSubPixelSize);
  // }
}

void Renderer::renderPixel(World &world, std::pair<int, int> &pixel,
                           int sqrtRaysPerPixel, float halfSubPixelSize)
{
  glm::vec3 cShape(0.f, 0.f, 0.f);
  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    cShape += colourAt(cast, world, RAY_BOUNCE_LIMIT);
  }

  cShape *= 1.f / (float)RAYS_PER_PIXEL;

  this->canvas.writePixel(pixel.first, pixel.second, cShape);
}

glm::vec3 Renderer::colourAt(Ray &ray, World &world, short remaining)
{
  std::vector<Geometry::Intersection<Shape>> intersections =
      world.intersectRay(ray);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    Geometry::getIntersectionParameters<Shape>(*hit, ray, intersections);
    return shadeHit(hit, world, remaining);
  }
  return glm::vec3(0.f, 0.f, 0.f);
}

glm::vec3 Renderer::shadeHit(Geometry::Intersection<Shape> *hit, World &world,
                             short remaining)
{
  bool inShadow = this->isShadowed(hit->comps->overPoint, world);

  glm::vec3 surface(0.f);

  for (auto &light : world.lights)
  {
    surface += lighting(hit->shapePtr, light, hit->comps->overPoint,
                        hit->comps->eyev, hit->comps->normalv, inShadow);
  }

  glm::vec3 reflection = reflectColour(hit, world, remaining);
  glm::vec3 refraction = refractedColour(hit, world, remaining);

  if (hit->shapePtr->material->reflective > 0 &&
      hit->shapePtr->material->transparency > 0)
  {
    float reflectance = Geometry::schlick<Shape>(hit->comps);
    return surface + reflection * reflectance + refraction * (1 - reflectance);
  }
  return surface + reflection + refraction;
}

glm::vec3 Renderer::reflectColour(Geometry::Intersection<Shape> *hit,
                                  World &world, short remaining)
{
  if (hit->shapePtr->material->reflective == 0 || remaining <= 0)
    return glm::vec3(0.f, 0.f, 0.f);

  Ray reflectRay = Ray(hit->comps->overPoint, hit->comps->reflectv);
  return colourAt(reflectRay, world, remaining - 1) *
         hit->shapePtr->material->reflective;
}

glm::vec3 Renderer::refractedColour(Geometry::Intersection<Shape> *hit,
                                    World &world, short remaining)
{
  float nRatio = hit->comps->n1 / hit->comps->n2;
  float cosI = glm::dot(hit->comps->eyev, hit->comps->normalv);
  float sin2T = (nRatio * nRatio) * (1 - (cosI * cosI));

  if (hit->shapePtr->material->transparency == 0 || sin2T > 1 ||
      remaining <= 0)
  {
    return glm::vec3(0.f, 0.f, 0.f);
  }

  float cosT = std::sqrt(1.f - sin2T);
  glm::vec4 direction = hit->comps->normalv * ((nRatio * cosI) - cosT) -
                        (hit->comps->eyev * nRatio);

  Ray refractedRay(hit->comps->underPoint, direction);

  glm::vec3 colour = colourAt(refractedRay, world, remaining - 1);

  return colour * hit->shapePtr->material->transparency;
}

glm::vec3 Renderer::lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                             glm::vec4 &point, glm::vec4 &eyev,
                             glm::vec4 &normalv, bool &inShadow)
{
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 effectiveColour;

  if (shape->material->pattern == nullptr)
    effectiveColour = shape->material->colour * light->intensity;
  else
    effectiveColour = shape->patternAt(point) * light->intensity;

  // combine the surface color with the light's color/intensity​
  // glm::vec3 effectiveColour = material->colour * light->intensity;
  // compute the ambient contribution​
  glm::vec3 ambient = effectiveColour * shape->material->ambient;
  if (inShadow)
    return ambient;

  glm::vec4 lightv = glm::normalize(light->position - point);

  // light_dot_normal represents the cosine of the angle between the​
  // light vector and the normal vector. A negative number means the​
  // light is on the other side of the surface.​

  float lightDotNormal = glm::dot(lightv, normalv);
  if (lightDotNormal < 0)
  {
    diffuse = glm::vec3(0.f, 0.f, 0.f);
    specular = glm::vec3(0.f, 0.f, 0.f);
  }
  else
  {
    // compute the diffuse contribution​
    diffuse = effectiveColour * shape->material->diffuse * lightDotNormal;

    // reflect_dot_eye represents the cosine of the angle between the
    // reflection vector and the eye vector. A negative number means the
    // light reflects away from the eye.​
    glm::vec4 reflectv = glm::reflect(-lightv, normalv);
    float reflectDotEye = glm::dot(reflectv, eyev);

    if (reflectDotEye <= 0)
    {
      specular = glm::vec3(0.f, 0.f, 0.f);
    }
    else
    {
      // compute the specular contribution​
      float factor = std::pow(reflectDotEye, shape->material->shininess);
      specular = light->intensity * shape->material->specular * factor;
    }
  }

  return (ambient + diffuse + specular);
}

bool Renderer::isShadowed(glm::vec4 &point, World &world)
{
  glm::vec4 v = world.lights.at(0)->position - point;
  float distance = glm::length(v);
  glm::vec4 direction = glm::normalize(v);

  Ray ray = Ray(point, direction);
  std::vector<Geometry::Intersection<Shape>> intersections =
      world.intersectRay(ray);

  Geometry::Intersection<Shape> *hit = Geometry::hit<Shape>(intersections);
  if (hit && hit->t < distance)
  {
    return true;
  }

  return false;
}