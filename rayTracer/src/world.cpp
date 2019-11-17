#include "world.h"

World::World()
{
}

World::~World()
{
}

void World::addLight(std::shared_ptr<PointLight>& light) {
    this->lights.push_back(light);
}

void World::addSphere(Sphere& sphere) {
    ;
    // std::make_shared<Author>(std::move(t_author));
    this->spheres.push_back(std::make_shared<Sphere>(std::move(sphere)));
}

void World::addShape(Shape& shape) {
    try {
        std::shared_ptr<Sphere> sphere_ptr = std::make_shared<Sphere>(std::move(*dynamic_cast<Sphere*>(&shape)));
        this->spheres.push_back(sphere_ptr);
    }
    catch (std::bad_cast& bc) {
        std::cerr << "bad_cast caught: " << bc.what() << '\n';
        // return;
    }
}

bool World::isShadowed(glm::vec4 point) {
    glm::vec4 v = this->lights.at(0)->position - point;
    float distance = glm::length(v);
    glm::vec4 direction = glm::normalize(v);

    Ray ray = Ray(point, direction);
    std::vector<Geometry::Intersection> intersections = this->intersectRay(ray);

    Geometry::Intersection* hit = Geometry::hit(intersections);
    if (hit && hit->t < distance) {
        return true;
    }


    return false;
}

std::vector<Geometry::Intersection> World::intersectRay(Ray& ray) {
	std::vector<Geometry::Intersection> ret;

	for (auto& sphere: this->spheres) {
		std::vector<Geometry::Intersection> next = Geometry::intersectRaySphere(ray, *sphere);
		ret.insert( ret.end(), std::make_move_iterator(next.begin()), std::make_move_iterator(next.end()));
	}

	std::sort(ret.begin(), ret.end(), Geometry::compareIntersection);

	return ret;
}

glm::vec3 World::shadeHit(Geometry::Intersection* hit) {
    bool inShadow = this->isShadowed(hit->comps->point);
	return Geometry::lighting(hit->spherePtr->material, this->lights.at(0), hit->comps->point, hit->comps->eyev, hit->comps->normalv, inShadow);
}