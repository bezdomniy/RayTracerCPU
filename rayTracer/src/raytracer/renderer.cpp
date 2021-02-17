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
#endif // WITH_THREADS
#else
  moodycamel::ConcurrentQueue<std::pair<int, int>> q;
  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      q.enqueue(std::make_pair(x, y));
    }
  }
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

glm::dvec3 Renderer::colourAt(Ray &ray, World &world, short remaining)
{
  std::vector<Geometry::Intersection<Shape>> intersections =
      world.intersectRay(ray);
  Geometry::Intersection<Shape> *hit;

  if ((hit = Geometry::hit<Shape>(intersections)))
  {
    Geometry::getIntersectionParameters<Shape>(*hit, ray.origin, ray.direction, intersections);
    return shadeHit(hit, world, remaining);
  }
  return glm::dvec3(0.0, 0.0, 0.0);
}

glm::dvec3 Renderer::shadeHit(Geometry::Intersection<Shape> *hit, World &world,
                              short remaining)
{
  glm::dvec3 surface(0.0);

  for (auto &light : world.lights)
  {
    bool inShadow = this->isShadowed(hit->comps->overPoint, world, light);
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
  {
    return glm::dvec3(0.0, 0.0, 0.0);
  }

  Ray reflectRay = Ray(hit->comps->overPoint, hit->comps->reflectv);
  return colourAt(reflectRay, world, remaining - 1) *
         hit->shapePtr->material->reflective;
}

glm::dvec3 Renderer::refractedColour(Geometry::Intersection<Shape> *hit,
                                     World &world, short remaining)
{
  if (hit->shapePtr->material->transparency == 0 || remaining <= 0)
  {
    return glm::dvec3(0.0, 0.0, 0.0);
  }

  double nRatio = hit->comps->n1 / hit->comps->n2;
  double cosI = glm::dot(hit->comps->eyev, hit->comps->normalv);
  double sin2T = (nRatio * nRatio) * (1 - (cosI * cosI));

  if (sin2T > 1)
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

glm::dvec3 Renderer::lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                              glm::dvec4 &point, glm::dvec4 &eyev,
                              glm::dvec4 &normalv, bool &inShadow)
{
  glm::dvec3 diffuse;
  glm::dvec3 specular;
  glm::dvec3 effectiveColour;

  if (shape->material->pattern == nullptr)
    effectiveColour = shape->material->colour * light->intensity;
  else
    effectiveColour = shape->patternAt(point) * light->intensity;

  // combine the surface color with the light's color/intensity​
  // glm::dvec3 effectiveColour = material->colour * light->intensity;
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

  return (ambient + diffuse + specular);
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