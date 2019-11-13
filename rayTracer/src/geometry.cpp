#include "geometry.h"

using namespace Geometry;

#include <iostream>
#include "glm/gtx/string_cast.hpp"

std::vector<Intersection> Geometry::intersectRaySphere(Ray& ray, Sphere& sphere) 
{
	std::vector<Intersection> ret;

	glm::mat4 inverseTransformSphere = glm::affineInverse(sphere.transform);
	Ray transformedRay = ray.transform(inverseTransformSphere);

	// std::cout << transformedRay <<std::endl;

	glm::vec4 sphereToRay = transformedRay.origin - sphere.position;
	float a = glm::dot(transformedRay.direction, transformedRay.direction);
	float b = 2 * glm::dot(transformedRay.direction, sphereToRay);
	float c = glm::dot(sphereToRay, sphereToRay) - (sphere.radius * sphere.radius);
	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return ret;

	float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
	float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

	ret.push_back(Intersection{t1, std::make_shared<Sphere>(sphere)});
	ret.push_back(Intersection{t2, std::make_shared<Sphere>(sphere)});

	return ret;
}

bool compareIntersection(Intersection i1, Intersection i2) 
{ 
    return (i1.t < i2.t); 
} 

std::vector<Intersection> Geometry::intersectRayWorld(Ray& ray, World& world) {
	std::vector<Intersection> ret;

	for (auto& sphere: world.spheres) {
		std::vector<Intersection> next = intersectRaySphere(ray, *sphere);
		ret.insert( ret.end(), next.begin(), next.end() );
	}

	std::sort(ret.begin(), ret.end(), compareIntersection);

	return ret;
}



Intersection* Geometry::hit(std::vector<Intersection>& intersections) {
    int retIndex = 0;

    if (!intersections.empty()) {
        for (int i = 0; i < intersections.size(); i++) {
			// std::cout << intersections.at(i).t << " ";
            if (intersections.at(i).t > 0 && intersections.at(i).t < intersections.at(retIndex).t) {
                retIndex = i;
            }
        }

        if (intersections.at(retIndex).t > 0) {
            return &intersections.at(retIndex);
        }
    }
    return nullptr;
}

