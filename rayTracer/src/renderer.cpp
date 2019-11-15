#include "renderer.h"

Renderer::Renderer()
{
}

Renderer::Renderer(std::shared_ptr<Camera>& c)
{
    this->camera = c;
    this->canvas = Canvas(this->camera->hsize,this->camera->vsize);
}

Renderer::~Renderer()
{
}

void Renderer::render(World& world) {
    for (int i = 0; i < this->canvas.height; i++) {
        for (int j = 0; j < this->canvas.width; j++) {
            Ray cast = this->camera->rayForPixel(i,j);
            glm::vec3 cShape = colourAt(cast, world);
            this->canvas.writePixel(i,j,cShape);
        }
    }
}

glm::vec3 Renderer::colourAt(Ray& ray, World& world) {
    std::vector<Geometry::Intersection> intersections = Geometry::intersectRayWorld(ray, world);
    Geometry::Intersection* hit;

    if ((hit = Geometry::hit(intersections))) {
        Geometry::getIntersectionParameters(*hit, ray);
        return Geometry::shadeHit(hit, world);
    }
    return glm::vec3(0.f, 0.f, 0.f);
}