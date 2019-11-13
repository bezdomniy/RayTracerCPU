#include "canvas.h"
#include "projectile.h"
#include "environment.h"
#include "ray.h"
#include "sphere.h"
#include "geometry.h"
#include "renderer.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>


void drawCircle() {
	Canvas c(100, 100);

	glm::vec3 c1(1.f, 0.f, 0.f);

	glm::vec4 point(40.f, 0.f, 0.f, 1.f);

	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, glm::vec3(50.0f, 50.0f, 0.0f));
	glm::mat4 invTranslation = glm::affineInverse(translation);

	glm::quat quatRotation = glm::angleAxis(glm::radians(30.f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotation = glm::mat4_cast(quatRotation);

	std::cout << point.x << " " << point.y << std::endl;
	for (int i = 0; i < 12; i++) {
		point = translation * rotation * point;

		c.writePixel(int(point.x), int(point.y), c1);
		point = invTranslation * point;
	}

	c.writeToPPM("./test.ppm", true);
}

void drawProjectile() {
	Canvas c(900, 550);

	glm::vec3 c1(1.f, 0.f, 0.f);
	glm::vec3 c2(0.f, 0.f, 0.f);

	c.clear(c2);

	Projectile p(glm::vec4(0.f, 1.f, 0.f,1.f), glm::vec4(1.f, 1.8f, 0.f, 0.f), 11.25f);

	std::unique_ptr<Environment> environment = std::make_unique<Environment>(glm::vec4(0.f, -0.1f, 0.f, 0.f), glm::vec4(-0.01f, 0.f, 0.f, 0.f));
	p.setEnvironment(environment);

	do {
		c.writePixel(int(p.position.x), int(p.position.y), c1);
		p.tick();

	} while (p.position.y > 0.f);

	//c.writeToPPM("/home/ilia/dev/gamedev/sdl_game/rayTracer/tests/test.ppm", true);
	c.writeToPPM("./test.ppm", true);
}




int main(int argc, char const *argv[])
{
	// Sphere s(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.0);
	// Ray r(glm::vec4(-5.f, 0.f, 0.f, 1.f), glm::vec4(1.f, 0.f, 0.f, 0.f));
	// std::vector<Geometry::Intersection> dists = Geometry::raySphereIntersection(r, s);

	// Geometry::Intersection* hit = Geometry::hit(dists);
	// if (hit) std::cout << "hit: " << hit->t << std::endl;

	// Ray r(glm::vec4(1.f, 2.f, 3.f, 1.f), glm::vec4(0.f, 1.f, 0.f, 0.f));
	// std::cout << r << std::endl;
	// glm::mat4 m = glm::translate(glm::mat4(1.f), glm::vec3(3.f, 4.f, 5.f));
	// r.transform(m);
	// std::cout << r << std::endl;

	// Ray r2(glm::vec4(1.f, 2.f, 3.f, 1.f), glm::vec4(0.f, 1.f, 0.f, 0.f));
	// std::cout << r2 << std::endl;
	// glm::mat4 m2 = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 3.f, 4.f));
	// r2.transform(m2);
	// std::cout << r2 << std::endl;

	// Ray r3(glm::vec4(-5.f, 0.f, 0.f, 1.f), glm::vec4(1.f, 0.f, 0.f, 0.f));
	// Sphere s(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);

	// s.transform = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f)); // should be 3 and 7
	// // s.transform = glm::translate(glm::mat4(1.f), glm::vec3(0.f,5.f,0.f)); //should be none

	// std::optional<std::vector<Geometry::Intersection>> intersections = Geometry::intersectRaySphere(r3, s);

	// if (intersections.has_value()) {
	// 	for (auto& i: intersections.value()) {
	// 		std::cout << i.t << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	Sphere s(0, glm::vec4(0.f, 0.f, 0.f, 1.f), 9.f);
	float ambient = 0.1f;
	float diffuse = 0.9f;
	float specular = 0.9f;
	float shininess = 200.f;
	std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(1.f,0.2f,1.f),ambient,diffuse,specular,shininess);
	s.setMaterial(material);

	std::shared_ptr<PointLight> light = std::make_shared<PointLight>(0, glm::vec4(-10.f,-3.f,-3.f,1.f),glm::vec3(1.f,1.f,1.f));


	// s.transform = glm::scale(glm::mat4(1.f), glm::vec3(9.f, 9.f, 9.f));
	std::shared_ptr<Canvas> canvas = std::make_shared<Canvas>(500, 500);
	Renderer renderer(canvas);
	renderer.addLight(light);
	renderer.render(s);

	canvas->writeToPPM("./test.ppm", true);


    return 0;
}
