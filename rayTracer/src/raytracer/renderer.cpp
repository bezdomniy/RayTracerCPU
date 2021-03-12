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

  std::vector<Geometry::Intersection<Shape>> intersections;
  intersections.reserve(world.shapes.size() * 2);

  // std::unique_ptr<Geometry::IntersectionParameters> hitCompsBuffer = std::make_unique<Geometry::IntersectionParameters>();

  for (int i = 0; i < RAYS_PER_PIXEL; i++)
  {
    Ray cast = this->camera->rayForPixel(pixel.first, pixel.second, i,
                                         sqrtRaysPerPixel, halfSubPixelSize);
    cShape += rayColourAt(cast, world, intersections, RAY_BOUNCE_LIMIT);
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

#if !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)
  moodycamel::ConcurrentQueue<std::pair<int, int>> q;
  // std::cout << "height and width: " << this->canvas.height << ", " << this->canvas.width << std::endl;
  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      q.enqueue(std::make_pair(x, y));
    }
  }

  size_t t = std::thread::hardware_concurrency();
  std::thread threads[t];

  std::cout << "using " << t << " threads" << std::endl;

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
  glm::dvec3 cShape(0.0, 0.0, 0.0);

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
    auto scale = 1.0 / (double)RAYS_PER_PIXEL;
    cShape.r = glm::clamp(std::sqrt(scale * cShape.r), 0.0, 0.999);
    cShape.g = glm::clamp(std::sqrt(scale * cShape.g), 0.0, 0.999);
    cShape.b = glm::clamp(std::sqrt(scale * cShape.b), 0.0, 0.999);
  }
  else
  {
    cShape /= (double)RAYS_PER_PIXEL;
  }

  this->canvas.writePixel(pixel.first, pixel.second, cShape);
}

glm::dvec3 Renderer::pathColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  if (remaining < 0)
    return glm::dvec3(0, 0, 0);

  // intersections.clear();

  // std::vector<Geometry::Intersection<Shape>> intersections =

  world.intersectRay(ray, intersections);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    // Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);

    // // glm::dvec4 target = hit->comps->point + hit->comps->normalv + glm::dvec4(glm::ballRand(1.0), 0.0);
    // glm::dvec4 target = hit->comps->point + glm::dvec4(randomInHemisphere(hit->comps->normalv), 0.0);
    // Ray newRay(hit->comps->point, target - hit->comps->point);

    // return 0.5 * colourAt(newRay, world, remaining - 1);

    // TODO: this does a lot of memory allocation. Make intersectionparameters once and reuse.

    Geometry::IntersectionParameters comps = Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    auto scatterDirection = comps.normalv + glm::dvec4(glm::normalize(glm::ballRand(1.0)), 0.0);

    Ray newRay(comps.overPoint, scatterDirection);

    glm::dvec3 hitColour = lighting(hit->shapePtr, comps.overPoint);
    // hitCompsBuffer = std::move(hit->comps);
    // intersections.clear();
    return hit->shapePtr->material->emissiveness + hitColour * pathColourAt(newRay, world, intersections, remaining - 1);

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

  // Background light
  return glm::dvec3(0.0, 0.0, 0.0);

  // glm::dvec3 unit_direction = glm::normalize(ray.direction);
  // auto t = 0.5 * (unit_direction.y + 1.0);
  // return (1.0 - t) * glm::dvec3(1.0, 1.0, 1.0) + t * glm::dvec3(0.5, 0.7, 1.0);

  // return glm::dvec3(0.0, 0.0, 0.0);
}

glm::dvec3 Renderer::rayColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  // intersections.clear();
  // std::vector<Geometry::Intersection<Shape>> intersections =
  world.intersectRay(ray, intersections);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    // TODO: Geometry::getRefractiveIndexFromTo no longer working because this is now a copy, so pointer equality breaks!
    Geometry::IntersectionParameters comps = Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    glm::dvec3 hitColour = shadeHit(hit, comps, world, intersections, remaining);
    return hitColour;
  }
  return glm::dvec3(0.0, 0.0, 0.0);
}

// Geometry::Intersection<Shape> hitCopy{hit->t, hit->shapePtr, hit->uv};
// hitCopy.comps = std::move(hitCompsBuffer);
// Geometry::getIntersectionParameters<Shape>(hitCopy, ray.origin, ray.direction, intersections);
// glm::dvec3 hitColour = shadeHit(&hitCopy, world, intersections, remaining);
// hitCompsBuffer = std::move(hitCopy.comps);

glm::dvec3 Renderer::shadeHit(Geometry::Intersection<Shape> *hit, Geometry::IntersectionParameters &comps, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  glm::dvec3 surface(0.0);

  double reflective = hit->shapePtr->material->reflective;
  double transparency = hit->shapePtr->material->transparency;
  Shape *shapePtr = hit->shapePtr;

  for (auto &light : world.lights)
  {
    bool inShadow = this->isShadowed(comps.overPoint, intersections, world, light);
    //     bool inShadow = false;
    surface += lighting(shapePtr, light, comps.overPoint,
                        comps.eyev, comps.normalv, inShadow);
  }

  bool doSchlick = reflective > 0 && transparency > 0;
  double reflectance = 0.0;
  if (doSchlick)
    reflectance = Geometry::schlick<Shape>(comps);

  glm::dvec3 reflection = reflectColour(comps, reflective, world, intersections, remaining);
  glm::dvec3 refraction = refractedColour(comps, transparency, world, intersections, remaining);

  if (doSchlick)
  {
    return surface + reflection * reflectance + refraction * (1 - reflectance);
  }
  return surface + reflection + refraction;
}

glm::dvec3 Renderer::reflectColour(Geometry::IntersectionParameters &comps, double reflective,
                                   World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  //  intersections.clear();
  if (reflective == 0 || remaining <= 0)
    return glm::dvec3(0.0, 0.0, 0.0);

  Ray reflectRay = Ray(comps.overPoint, comps.reflectv);

  // std::unique_ptr<Geometry::IntersectionParameters> hitCompsBuffer = std::move(hit->comps);
  glm::dvec3 reflectionColour = rayColourAt(reflectRay, world, intersections, remaining - 1) *
                                reflective;
  // hit->comps = std::move(hitCompsBuffer);

  return reflectionColour;
}

glm::dvec3 Renderer::refractedColour(Geometry::IntersectionParameters &comps, double transparency,
                                     World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining)
{
  if (transparency == 0 || remaining <= 0)
  {
    return glm::dvec3(0.0, 0.0, 0.0);
  }

  double nRatio = comps.n1 / comps.n2;
  double cosI = glm::dot(comps.eyev, comps.normalv);
  double sin2T = (nRatio * nRatio) * (1 - (cosI * cosI));

  if (sin2T > 1)
  {
    return glm::dvec3(0.0, 0.0, 0.0);
  }

  double cosT = std::sqrt(1.0 - sin2T);
  glm::dvec4 direction = comps.normalv * ((nRatio * cosI) - cosT) -
                         (comps.eyev * nRatio);

  Ray refractedRay(comps.underPoint, direction);

  glm::dvec3 colour = rayColourAt(refractedRay, world, intersections, remaining - 1);
  // hit->comps = std::move(hitCompsBuffer);

  return colour * transparency;
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
  glm::dvec3 specular;
  glm::dvec3 effectiveColour;

  // combine the surface color with the light's color/intensity​
  if (shape->material->pattern == nullptr)
    effectiveColour = shape->material->colour * light->intensity;
  else
    effectiveColour = shape->patternAt(point) * light->intensity;

  // compute the ambient contribution​
  glm::dvec3 ambient = effectiveColour * shape->material->ambient;
  if (inShadow)
    return ambient;

  glm::dvec4 lightv = glm::normalize(light->position - point);

  // light_dot_normal represents the cosine of the angle between the​
  // light vector and the normal vector. A negative number means the​
  // light is on the other side of the surface.​

  double lightDotNormal = glm::dot(lightv, normalv);
  if (lightDotNormal < 0)
  {
    diffuse = glm::dvec3(0.0, 0.0, 0.0);
    specular = glm::dvec3(0.0, 0.0, 0.0);
  }
  else
  {
    // compute the diffuse contribution​
    diffuse = effectiveColour * shape->material->diffuse * lightDotNormal;

    // reflect_dot_eye represents the cosine of the angle between the
    // reflection vector and the eye vector. A negative number means the
    // light reflects away from the eye.​
    glm::dvec4 reflectv = glm::reflect(-lightv, normalv);
    double reflectDotEye = glm::dot(reflectv, eyev);

    if (reflectDotEye <= 0)
    {
      specular = glm::dvec3(0.0, 0.0, 0.0);
    }
    else
    {
      // compute the specular contribution​
      double factor = std::pow(reflectDotEye, shape->material->shininess);
      specular = light->intensity * shape->material->specular * factor;
    }
  }

  return ambient + diffuse + specular;
}

bool Renderer::isShadowed(glm::dvec4 &point, std::vector<Geometry::Intersection<Shape>> &intersections, World &world,
                          std::shared_ptr<PointLight> &light)
{
  // intersections.clear();
  glm::dvec4 v = light->position - point;
  double distance = glm::length(v);
  glm::dvec4 direction = glm::normalize(v);

  Ray ray = Ray(point, direction);
  return world.intersectRayShadow(ray, intersections, distance);
}
