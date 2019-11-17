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
    for (int y = 0; y < this->canvas.height; y++) {
        for (int x = 0; x < this->canvas.width; x++) {
            Ray cast = this->camera->rayForPixel(x,y);
            glm::vec3 cShape = colourAt(cast, world);
            this->canvas.writePixel(x,y,cShape);
        }
    }
}

glm::vec3 Renderer::colourAt(Ray& ray, World& world) {
    std::vector<Geometry::Intersection> intersections = world.intersectRay(ray);
    Geometry::Intersection* hit;

    if ((hit = Geometry::hit(intersections))) {
        Geometry::getIntersectionParameters(*hit, ray);
        return world.shadeHit(hit);
    }
    return glm::vec3(0.f, 0.f, 0.f);
}