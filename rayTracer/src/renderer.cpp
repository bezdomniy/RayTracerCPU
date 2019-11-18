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
  // this->world = std::make_shared<World>(world);
  for (int y = 0; y < this->canvas.height; y++)
  {
    for (int x = 0; x < this->canvas.width; x++)
    {
      Ray cast = this->camera->rayForPixel(x, y);
      glm::vec3 cShape = colourAt(cast, world);
      this->canvas.writePixel(x, y, cShape);
    }
  }
}

glm::vec3 Renderer::colourAt(Ray &ray, World &world)
{
  std::vector<Geometry::Intersection> intersections = world.intersectRay(ray);
  Geometry::Intersection *hit;

  if ((hit = Geometry::hit(intersections)))
  {
    Geometry::getIntersectionParameters(*hit, ray);
    return shadeHit(hit, world);
  }
  return glm::vec3(0.f, 0.f, 0.f);
}

glm::vec3 Renderer::lighting(std::shared_ptr<Material> material,
                             std::shared_ptr<PointLight> light, glm::vec4 point,
                             glm::vec4 eyev, glm::vec4 normalv, bool inShadow)
{
  glm::vec3 diffuse;
  glm::vec3 specular;

  // combine the surface color with the light's color/intensity​
  glm::vec3 effectiveColour = material->colour * light->intensity;
  // compute the ambient contribution​
  glm::vec3 ambient = effectiveColour * material->ambient;
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
    diffuse = effectiveColour * material->diffuse * lightDotNormal;

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
      float factor = std::pow(reflectDotEye, material->shininess);
      specular = light->intensity * material->specular * factor;
    }
  }

  return (ambient + diffuse + specular);
}

glm::vec3 Renderer::shadeHit(Geometry::Intersection *hit, World &world)
{
  bool inShadow = this->isShadowed(hit->comps->overPoint, world);
  return lighting(hit->shapePtr->material, world.lights.at(0),
                  hit->comps->overPoint, hit->comps->eyev,
                  hit->comps->normalv, inShadow);
}

bool Renderer::isShadowed(glm::vec4 point, World &world)
{
  glm::vec4 v = world.lights.at(0)->position - point;
  float distance = glm::length(v);
  glm::vec4 direction = glm::normalize(v);

  Ray ray = Ray(point, direction);
  std::vector<Geometry::Intersection> intersections = world.intersectRay(ray);

  Geometry::Intersection *hit = Geometry::hit(intersections);
  if (hit && hit->t < distance)
  {
    return true;
  }

  return false;
}