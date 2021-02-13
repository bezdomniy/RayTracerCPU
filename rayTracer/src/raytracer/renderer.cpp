#include "renderer.h"

Renderer::Renderer() {}

Renderer::Renderer(std::shared_ptr<Camera> &c)
{
  this->camera = c;
  this->canvas = Canvas(this->camera->hsize, this->camera->vsize);
}

#ifdef __EMSCRIPTEN__ //TODO change to if emscripten AND if use_threaded
Renderer::Renderer(std::shared_ptr<Camera> &c, uint8_t nWorkers)
{
  this->camera = c;
  this->canvas = Canvas(this->camera->hsize, (this->camera->vsize / nWorkers) + 1);
}

void Renderer::renderPixel(World &world, std::pair<int, int> &pixel, uint8_t nWorkers)
{
  // std::cout << "rendering: " << pixel.first << ", " << pixel.second << std::endl;
  glm::dvec3 cShape(0.0, 0.0, 0.0);
  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    cShape += colourAt(cast, world, RAY_BOUNCE_LIMIT);
  }

  cShape *= 1.0 / (double)RAYS_PER_PIXEL;

  this->canvas.writePixel(pixel.first, pixel.second / nWorkers, cShape);
}
#endif

Renderer::~Renderer()
{
}

void Renderer::render(World &world)
{
  // std::vector<std::pair<int, int>> pixels;
  // pixels.reserve(this->canvas.height * this->canvas.width);

  // for (int y = 0; y < this->canvas.height; y++)
  // {
  //   for (int x = 0; x < this->canvas.width; x++)
  //   {
  //     pixels.push_back(std::make_pair(x, y));
  //   }
  // }

  moodycamel::ConcurrentQueue<std::pair<int, int>> q;
  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      q.enqueue(std::make_pair(x, y));
    }
  }

  // std::random_device rd;
  // std::mt19937 g(rd());

  // std::shuffle(pixels.begin(), pixels.end(), g);

#ifdef __EMSCRIPTEN__
#ifdef WITH_THREADS
  size_t t = std::thread::hardware_concurrency();
  std::thread threads[t];
  for (int i = 0; i < t; ++i)
  {
    threads[i] = std::thread([&]() {
      std::pair<int, int> pixel;
      while (q.try_dequeue(pixel))
      {
        renderPixel(world, pixel);
      }
    });
  }

  // Wait for all threads
  for (int i = 0; i < t; ++i)
  {
    threads[i].join();
  }

  std::pair<int, int> pixel;
  while (q.try_dequeue(pixel))
  {
    renderPixel(world, pixel);
  }
  // tf::Executor executor;
  // tf::Taskflow taskflow;

  // taskflow.for_each(
  //     pixels.begin(), pixels.end(),
  //     [this, &world](auto &pixel) {
  //       std::cout << "rendering: " << pixel.first << ", " << pixel.second << std::endl;
  //       // this->renderPixel(world, pixel);
  //     });
  // std::cout << "starting thread" << std::endl;
  // // executor.run(taskflow).wait();
  // executor.run(taskflow);
  // executor.wait_for_all();
  // std::cout << "done thread" << std::endl;
#else
  for (std::vector<std::pair<int, int>>::iterator it = pixels.begin();
       it != pixels.end(); ++it)
  {
    renderPixel(world, *it);
  }
#endif // WITH_THREADS
#else

  size_t t = std::thread::hardware_concurrency();
  std::thread threads[t];
  for (int i = 0; i < t; ++i)
  {
    threads[i] = std::thread([&]() {
      std::pair<int, int> pixel;
      while (q.try_dequeue(pixel))
      {
        renderPixel(world, pixel);
      }
    });
  }

  // Wait for all threads
  for (int i = 0; i < t; ++i)
  {
    threads[i].join();
  }

  std::pair<int, int> pixel;
  while (q.try_dequeue(pixel))
  {
    renderPixel(world, pixel);
  }

  // std::for_each(
  //     std::execution::par_unseq, pixels.begin(), pixels.end(),
  //     [this, &world](auto &&pixel) {
  //       renderPixel(world, pixel);
  //     });
  //
  // for (std::vector<std::pair<int, int>>::iterator it = pixels.begin();
  //      it != pixels.end(); ++it)
  // {
  //   renderPixel(world, *it);
  // }
  //
  // #pragma omp parallel for
  // for (std::vector<std::pair<int, int>>::iterator it = pixels.begin();
  //     it < pixels.end(); ++it)
  // {
  //     renderPixel(world, *it);
  // }
#endif
}

void Renderer::renderPixel(World &world, const std::pair<int, int> &pixel)
{
  // std::cout << "rendering: " << pixel.first << ", " << pixel.second << std::endl;
  glm::dvec3 cShape(0.0, 0.0, 0.0);
  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    cShape += colourAt(cast, world, RAY_BOUNCE_LIMIT);
  }

  // auto scale = 1.0 / (double)RAYS_PER_PIXEL;
  // cShape.r = glm::clamp(std::sqrt(scale * cShape.r), 0.0, 0.999);
  // cShape.g = glm::clamp(std::sqrt(scale * cShape.g), 0.0, 0.999);
  // cShape.b = glm::clamp(std::sqrt(scale * cShape.b), 0.0, 0.999);

  cShape /= (double)RAYS_PER_PIXEL;

  this->canvas.writePixel(pixel.first, pixel.second, cShape);
}

inline glm::dvec3 randomInHemisphere(glm::dvec4 &normalv)
{
  glm::dvec3 inUnitSphere = glm::ballRand(1.0);
  if (glm::dot(inUnitSphere, glm::dvec3(normalv)) > 0.0) // In the same hemisphere as the normal
    return inUnitSphere;
  else
    return -inUnitSphere;
}

glm::dvec3 Renderer::colourAt(Ray &ray, World &world, short remaining)
{
  if (remaining < 0)
    return glm::dvec3(0, 0, 0);

  std::vector<Geometry::Intersection<Shape>> intersections =
      world.intersectRay(ray);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    // Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);

    // // glm::dvec4 target = hit->comps->point + hit->comps->normalv + glm::dvec4(glm::ballRand(1.0), 0.0);
    // glm::dvec4 target = hit->comps->point + glm::dvec4(randomInHemisphere(hit->comps->normalv), 0.0);
    // Ray newRay(hit->comps->point, target - hit->comps->point);

    // return 0.5 * colourAt(newRay, world, remaining - 1);

    Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    auto scatterDirection = hit->comps->normalv + glm::dvec4(glm::normalize(glm::sphericalRand(1.0)), 0.0);

    Ray newRay(hit->comps->point, scatterDirection);
    return shadeHit(hit, world, remaining) * colourAt(newRay, world, remaining - 1);

    // return shadeHit(hit, world, remaining);
  }

  // Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
  // auto scatterDirection = hit->comps->normalv + glm::dvec4(glm::normalize(glm::sphericalRand(1.0)), 0.0);

  // Ray newRay(hit->comps->point, scatterDirection);
  // glm::dvec3 effectiveColour;

  // if (hit->shapePtr->material->pattern == nullptr)
  //   effectiveColour = hit->shapePtr->material->colour;
  // else
  //   effectiveColour = hit->shapePtr->patternAt(hit->comps->point);

  return glm::dvec3(1.0, 1.0, 1.0);

  // glm::dvec3 unit_direction = glm::normalize(ray.direction);
  // auto t = 0.5 * (unit_direction.y + 1.0);
  // return (1.0 - t) * glm::dvec3(1.0, 1.0, 1.0) + t * glm::dvec3(0.5, 0.7, 1.0);

  // return glm::dvec3(0.0, 0.0, 0.0);
}

glm::dvec3 Renderer::shadeHit(Geometry::Intersection<Shape> *hit, World &world,
                              short remaining)
{
  glm::dvec3 surface(0.0);

  for (auto &light : world.lights)
  {
    // bool inShadow = this->isShadowed(hit->comps->overPoint, world, light);
    bool inShadow = false;
    surface += lighting(hit->shapePtr, light, hit->comps->overPoint,
                        hit->comps->eyev, hit->comps->normalv, inShadow);
  }

  glm::dvec3 reflection = reflectColour(hit, world, remaining);
  glm::dvec3 refraction = refractedColour(hit, world, remaining);

  if (hit->shapePtr->material->reflective > 0 &&
      hit->shapePtr->material->transparency > 0)
  {
    double reflectance = Geometry::schlick<Shape>(hit->comps);
    return surface + reflection * reflectance + refraction * (1 - reflectance);
  }
  return surface + reflection + refraction;
}

glm::dvec3 Renderer::reflectColour(Geometry::Intersection<Shape> *hit,
                                   World &world, short remaining)
{
  if (hit->shapePtr->material->reflective == 0 || remaining <= 0)
    return glm::dvec3(0.0, 0.0, 0.0);

  Ray reflectRay = Ray(hit->comps->overPoint, hit->comps->reflectv);
  return colourAt(reflectRay, world, remaining - 1) *
         hit->shapePtr->material->reflective;
}

glm::dvec3 Renderer::refractedColour(Geometry::Intersection<Shape> *hit,
                                     World &world, short remaining)
{
  double nRatio = hit->comps->n1 / hit->comps->n2;
  double cosI = glm::dot(hit->comps->eyev, hit->comps->normalv);
  double sin2T = (nRatio * nRatio) * (1 - (cosI * cosI));

  if (hit->shapePtr->material->transparency == 0 || sin2T > 1 ||
      remaining <= 0)
  {
    return glm::dvec3(0.0, 0.0, 0.0);
  }

  double cosT = std::sqrt(1.0 - sin2T);
  glm::dvec4 direction = hit->comps->normalv * ((nRatio * cosI) - cosT) -
                         (hit->comps->eyev * nRatio);

  Ray refractedRay(hit->comps->underPoint, direction);

  glm::dvec3 colour = colourAt(refractedRay, world, remaining - 1);

  return colour * hit->shapePtr->material->transparency;
}

glm::dvec3 Renderer::lighting(Shape *shape, glm::dvec4 &point)
{
  glm::dvec3 effectiveColour;

  if (shape->material->pattern == nullptr)
    effectiveColour = shape->material->colour;
  else
    effectiveColour = shape->patternAt(point);

  return effectiveColour;
}

glm::dvec3
Renderer::lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                   glm::dvec4 &point, glm::dvec4 &eyev,
                   glm::dvec4 &normalv, bool &inShadow)
{
  glm::dvec3 diffuse;
  // glm::dvec3 specular;
  glm::dvec3 effectiveColour;

  if (shape->material->pattern == nullptr)
    effectiveColour = shape->material->colour * light->intensity;
  else
    effectiveColour = shape->patternAt(point) * light->intensity;

  // combine the surface color with the light's color/intensity​
  // glm::dvec3 effectiveColour = material->colour * light->intensity;
  // compute the ambient contribution​
  // glm::dvec3 ambient = effectiveColour * shape->material->ambient;
  // if (inShadow)
  //   return ambient;

  glm::dvec4 lightv = glm::normalize(light->position - point);

  // light_dot_normal represents the cosine of the angle between the​
  // light vector and the normal vector. A negative number means the​
  // light is on the other side of the surface.​

  double lightDotNormal = glm::dot(lightv, normalv);
  if (lightDotNormal < 0)
  {
    diffuse = glm::dvec3(0.0, 0.0, 0.0);
    // specular = glm::dvec3(0.0, 0.0, 0.0);
  }
  else
  {
    // compute the diffuse contribution​
    diffuse = effectiveColour * shape->material->diffuse * lightDotNormal;

    // reflect_dot_eye represents the cosine of the angle between the
    // reflection vector and the eye vector. A negative number means the
    // light reflects away from the eye.​
    // glm::dvec4 reflectv = glm::reflect(-lightv, normalv);
    // double reflectDotEye = glm::dot(reflectv, eyev);

    // if (reflectDotEye <= 0)
    // {
    //   specular = glm::dvec3(0.0, 0.0, 0.0);
    // }
    // else
    // {
    //   // compute the specular contribution​
    //   double factor = std::pow(reflectDotEye, shape->material->shininess);
    //   specular = light->intensity * shape->material->specular * factor;
    // }
  }

  return diffuse; //(ambient + diffuse + specular);
}

bool Renderer::isShadowed(glm::dvec4 &point, World &world,
                          std::shared_ptr<PointLight> &light)
{

  glm::dvec4 v = light->position - point;
  double distance = glm::length(v);
  glm::dvec4 direction = glm::normalize(v);

  Ray ray = Ray(point, direction);
  std::vector<Geometry::Intersection<Shape>> intersections =
      world.intersectRayShadow(ray);

  Geometry::Intersection<Shape> *hit = Geometry::hit<Shape>(intersections);
  if ((hit && hit->t < distance))
  {
    return true;
  }

  return false;
}