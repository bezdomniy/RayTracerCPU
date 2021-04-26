#include "renderer.h"

Renderer::Renderer() {}

Renderer::Renderer(std::shared_ptr<Camera> &c)
{
  this->camera = c;
  this->canvas = Canvas(this->camera->hsize, this->camera->vsize);

  //    this->t = std::thread::hardware_concurrency();
  //    this->threads.reserve(this->t);
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
  Vec3 cShape(0.0, 0.0, 0.0);

  std::vector<Geometry::Intersection<Shape>> intersections;
  intersections.reserve(world.shapes.size() * 2);

  // std::unique_ptr<Geometry::IntersectionParameters> hitCompsBuffer = std::make_unique<Geometry::IntersectionParameters>();

  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    cShape += rayColourAt(cast, world, intersections, RAY_BOUNCE_LIMIT);
  }

  cShape *= 1.0 / (Float)RAYS_PER_PIXEL;

  this->canvas.writePixel(pixel.first, pixel.second / nWorkers, cShape);
}
#endif

Renderer::~Renderer()
{
}

void Renderer::render(World &world)
{

#if !defined(__EMSCRIPTEN__) || defined(WITH_THREADS)
  moodycamel::ConcurrentQueue<std::pair<int, int>> q;
  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      q.enqueue(std::make_pair(x, y));
    }
  }

  size_t t = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(t);
  for (int i = 0; i < t; ++i)
  {
    threads.push_back(std::thread([&]() {
      std::pair<int, int> pixel;
      while (q.try_dequeue(pixel))
      {
        renderPixel(world, pixel);
      }
    }));
  }

  // Wait for all threads
  //  for (int i = 0; i < t; ++i)
  //  {
  //    threads[i].join();
  //  }
  while (!threads.empty())
  {
    threads.back().join();
    threads.pop_back();
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
#else
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

  for (std::vector<std::pair<int, int>>::iterator it = pixels.begin();
       it != pixels.end(); ++it)
  {
    renderPixel(world, *it);
  }
#endif
}

void Renderer::renderPixel(World &world, const std::pair<int, int> &pixel)
{
  // std::cout << "rendering: " << pixel.first << ", " << pixel.second << std::endl;
  Vec3 cShape(0.0, 0.0, 0.0);

  std::vector<Geometry::Intersection<Shape>> intersections;
  intersections.reserve(world.shapes.size() * 2);

  //  std::unique_ptr<Geometry::IntersectionParameters> hitCompsBuffer = std::make_unique<Geometry::IntersectionParameters>();

  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    // intersections.clear();

    if (isPathTracer)
      cShape += pathColourAt(cast, world, intersections, RAY_BOUNCE_LIMIT);
    else
      cShape += rayColourAt(cast, world, intersections, RAY_BOUNCE_LIMIT);
  }

  if (isPathTracer)
  {
    auto scale = 1.0 / (Float)RAYS_PER_PIXEL;
    cShape.r = glm::clamp(std::sqrt(scale * cShape.r), 0.0, 0.999);
    cShape.g = glm::clamp(std::sqrt(scale * cShape.g), 0.0, 0.999);
    cShape.b = glm::clamp(std::sqrt(scale * cShape.b), 0.0, 0.999);
  }
  else
  {
    cShape /= (Float)RAYS_PER_PIXEL;
  }

  this->canvas.writePixel(pixel.first, pixel.second, cShape);
}

Vec3 Renderer::pathColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  if (remaining < 0)
    return Vec3(0, 0, 0);

  // intersections.clear();

  // std::vector<Geometry::Intersection<Shape>> intersections =

  world.intersectRay(ray, intersections);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    // Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);

    // // Vec4 target = hit->comps->point + hit->comps->normalv + Vec4(glm::ballRand(1.0), 0.0);
    // Vec4 target = hit->comps->point + Vec4(randomInHemisphere(hit->comps->normalv), 0.0);
    // Ray newRay(hit->comps->point, target - hit->comps->point);

    // return 0.5 * colourAt(newRay, world, remaining - 1);

    // TODO: this does a lot of memory allocation. Make intersectionparameters once and reuse.

    Geometry::IntersectionParameters comps = Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    // TODO: check if this scatter function is right
    auto scatterDirection = comps.normalv + Vec4(glm::normalize(glm::ballRand(1.0)), 0.0);

    // TODO: catch near zero case

    Ray newRay(comps.overPoint, scatterDirection);

    Vec3 hitColour = lighting(hit->shapePtr, comps.overPoint);
    // hitCompsBuffer = std::move(hit->comps);
    // intersections.clear();
    return hit->shapePtr->getMaterial()->emissiveness + hitColour * pathColourAt(newRay, world, intersections, remaining - 1);

    // return shadeHit(hit, world, remaining);
  }

  // Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
  // auto scatterDirection = hit->comps->normalv + Vec4(glm::normalize(glm::sphericalRand(1.0)), 0.0);

  // Ray newRay(hit->comps->point, scatterDirection);
  // Vec3 effectiveColour;

  // if (hit->shapePtr->material->pattern == nullptr)
  //   effectiveColour = hit->shapePtr->material->colour;
  // else
  //   effectiveColour = hit->shapePtr->patternAt(hit->comps->point);

  // Background light
  return Vec3(0.0, 0.0, 0.0);

  // Vec3 unit_direction = glm::normalize(ray.direction);
  // auto t = 0.5 * (unit_direction.y + 1.0);
  // return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);

  // return Vec3(0.0, 0.0, 0.0);
}

Vec3 Renderer::rayColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  // intersections.clear();
  // std::vector<Geometry::Intersection<Shape>> intersections =
  world.intersectRay(ray, intersections);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    Geometry::IntersectionParameters comps = Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    Vec3 hitColour = shadeHit(hit, comps, world, intersections, remaining);
    return hitColour;
  }
  return Vec3(0.0, 0.0, 0.0);
}

// Geometry::Intersection<Shape> hitCopy{hit->t, hit->shapePtr, hit->uv};
// hitCopy.comps = std::move(hitCompsBuffer);
// Geometry::getIntersectionParameters<Shape>(hitCopy, ray.origin, ray.direction, intersections);
// Vec3 hitColour = shadeHit(&hitCopy, world, intersections, remaining);
// hitCompsBuffer = std::move(hitCopy.comps);

Vec3 Renderer::shadeHit(Geometry::Intersection<Shape> *hit, Geometry::IntersectionParameters &comps, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  Vec3 surface(0.0);

  Float reflective = hit->shapePtr->getMaterial()->reflective;
  Float transparency = hit->shapePtr->getMaterial()->transparency;
  Shape *shapePtr = hit->shapePtr;

  for (auto &light : world.lights)
  {
    bool inShadow = this->isShadowed(comps.overPoint, intersections, world, light);
    //     bool inShadow = false;
    surface += lighting(shapePtr, light, comps.overPoint,
                        comps.eyev, comps.normalv, inShadow);
  }

  bool doSchlick = reflective > 0 && transparency > 0;
  Float reflectance = 0.0;
  if (doSchlick)
    reflectance = Geometry::schlick<Shape>(comps);

  Vec3 reflection = reflectColour(comps, reflective, world, intersections, remaining);
  Vec3 refraction = refractedColour(comps, transparency, world, intersections, remaining);

  if (doSchlick)
  {
    return surface + reflection * reflectance + refraction * (1 - reflectance);
  }
  return surface + reflection + refraction;
}

Vec3 Renderer::reflectColour(Geometry::IntersectionParameters &comps, Float reflective,
                             World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  //  intersections.clear();
  if (reflective == 0 || remaining <= 0)
    return Vec3(0.0, 0.0, 0.0);

  Ray reflectRay = Ray(comps.overPoint, comps.reflectv);

  // std::unique_ptr<Geometry::IntersectionParameters> hitCompsBuffer = std::move(hit->comps);
  Vec3 reflectionColour = rayColourAt(reflectRay, world, intersections, remaining - 1) *
                          reflective;
  // hit->comps = std::move(hitCompsBuffer);

  return reflectionColour;
}

Vec3 Renderer::refractedColour(Geometry::IntersectionParameters &comps, Float transparency,
                               World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  if (transparency == 0 || remaining <= 0)
  {
    return Vec3(0.0, 0.0, 0.0);
  }

  Float nRatio = comps.n1 / comps.n2;
  Float cosI = glm::dot(comps.eyev, comps.normalv);
  Float sin2T = (nRatio * nRatio) * (1 - (cosI * cosI));

  if (sin2T > 1)
  {
    return Vec3(0.0, 0.0, 0.0);
  }

  Float cosT = std::sqrt(1.0 - sin2T);
  Vec4 direction = comps.normalv * ((nRatio * cosI) - cosT) -
                   (comps.eyev * nRatio);

  Ray refractedRay(comps.underPoint, direction);

  Vec3 colour = rayColourAt(refractedRay, world, intersections, remaining - 1);
  // hit->comps = std::move(hitCompsBuffer);

  return colour * transparency;
}

Vec3 Renderer::lighting(Shape *shape, Vec4 &point)
{
  Vec3 effectiveColour;

  if (shape->getMaterial()->pattern == nullptr)
    effectiveColour = shape->getMaterial()->colour;
  else
    effectiveColour = shape->patternAt(point);

  return effectiveColour;
}

Vec3 Renderer::lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                        Vec4 &point, Vec4 &eyev,
                        Vec4 &normalv, bool &inShadow)
{
  Vec3 diffuse;
  Vec3 specular;
  Vec3 effectiveColour;

  // combine the surface color with the light's color/intensity​
  if (shape->getMaterial()->pattern == nullptr)
    effectiveColour = shape->getMaterial()->colour * light->intensity;
  else
    effectiveColour = shape->patternAt(point) * light->intensity;

  // compute the ambient contribution​
  Vec3 ambient = effectiveColour * shape->getMaterial()->ambient;
  if (inShadow)
    return ambient;

  Vec4 lightv = glm::normalize(light->position - point);

  // light_dot_normal represents the cosine of the angle between the​
  // light vector and the normal vector. A negative number means the​
  // light is on the other side of the surface.​

  Float lightDotNormal = glm::dot(lightv, normalv);
  if (lightDotNormal < 0)
  {
    diffuse = Vec3(0.0, 0.0, 0.0);
    specular = Vec3(0.0, 0.0, 0.0);
  }
  else
  {
    // compute the diffuse contribution​
    diffuse = effectiveColour * shape->getMaterial()->diffuse * lightDotNormal;

    // reflect_dot_eye represents the cosine of the angle between the
    // reflection vector and the eye vector. A negative number means the
    // light reflects away from the eye.​
    Vec4 reflectv = glm::reflect(-lightv, normalv);
    Float reflectDotEye = glm::dot(reflectv, eyev);

    if (reflectDotEye <= 0)
    {
      specular = Vec3(0.0, 0.0, 0.0);
    }
    else
    {
      // compute the specular contribution​
      Float factor = std::pow(reflectDotEye, shape->getMaterial()->shininess);
      specular = light->intensity * shape->getMaterial()->specular * factor;
    }
  }

  return ambient + diffuse + specular;
}

bool Renderer::isShadowed(Vec4 &point, std::vector<Geometry::Intersection<Shape>> &intersections, World &world,
                          std::shared_ptr<PointLight> &light)
{
  // intersections.clear();
  Vec4 v = light->position - point;
  Float distance = glm::length(v);
  Vec4 direction = glm::normalize(v);

  Ray ray = Ray(point, direction);
  return world.intersectRayShadow(ray, intersections, distance);
}
