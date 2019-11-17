#include "geometry.h"

using namespace Geometry;

#include <iostream>
#include "glm/gtx/string_cast.hpp"

void Geometry::getIntersectionParameters(Intersection& intersection, Ray& ray) {
	intersection.comps = std::make_unique<IntersectionParameters>();
	intersection.comps->point = ray.origin + glm::normalize(ray.direction) * intersection.t;
	intersection.comps->normalv = intersection.spherePtr->normalAt(intersection.comps->point);
	intersection.comps->eyev = -ray.direction;

	if (glm::dot(intersection.comps->normalv,intersection.comps->eyev) < 0) {
		intersection.comps->inside = true;
		intersection.comps->normalv = -intersection.comps->normalv;
	}
	else {
		intersection.comps->inside = false;
	}
}

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

bool Geometry::compareIntersection(Intersection& i1, Intersection& i2) 
{ 
    return (i1.t < i2.t); 
} 



Intersection* Geometry::hit(std::vector<Intersection>& intersections) {
    int retIndex = -1;

    if (!intersections.empty()) {
        for (int i = 0; i < intersections.size(); i++) {
			// std::cout << intersections.at(i).t << " ";
            if (retIndex == -1 && intersections.at(i).t > 0) {
                retIndex = i;
            }
            else if (intersections.at(i).t > 0 && intersections.at(i).t < intersections.at(retIndex).t) {
                retIndex = i;
            }
        }

        if (retIndex != -1 && intersections.at(retIndex).t > 0) {
            return &intersections.at(retIndex);
        }
    }
    return nullptr;
}

glm::vec3 Geometry::lighting(std::shared_ptr<Material> material, std::shared_ptr<PointLight> light, glm::vec4 point, glm::vec4 eyev, glm::vec4 normalv, bool inShadow) {
    glm::vec3 diffuse;
    glm::vec3 specular;

    //combine the surface color with the light's color/intensity​
    glm::vec3 effectiveColour = material->colour * light->intensity;
    //compute the ambient contribution​
    glm::vec3 ambient = effectiveColour * material->ambient;
    if (inShadow) return ambient;

    glm::vec4 lightv = glm::normalize(light->position - point);

    //light_dot_normal represents the cosine of the angle between the​
    //light vector and the normal vector. A negative number means the​
    //light is on the other side of the surface.​

    float lightDotNormal = glm::dot(lightv, normalv);
    if (lightDotNormal < 0) {
        diffuse = glm::vec3(0.f,0.f,0.f);
        specular = glm::vec3(0.f,0.f,0.f);
    }
    else { 
        //compute the diffuse contribution​
        diffuse = effectiveColour * material->diffuse * lightDotNormal;

        //reflect_dot_eye represents the cosine of the angle between the
        //reflection vector and the eye vector. A negative number means the
        //light reflects away from the eye.​
        glm::vec4 reflectv = glm::reflect(-lightv, normalv);
        float reflectDotEye = glm::dot(reflectv, eyev);

        if (reflectDotEye <= 0) {
            specular = glm::vec3(0.f,0.f,0.f);
        }
        else {
            //compute the specular contribution​
            float factor = std::pow(reflectDotEye, material->shininess);
            specular = light->intensity * material->specular * factor;
        }
    }

    return (ambient + diffuse + specular);
}




