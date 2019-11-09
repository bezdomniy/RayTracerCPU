#include "ray.h"

Ray::Ray(glm::vec4 origin, glm::vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

Ray::~Ray()
{
}

std::ostream& operator<< (std::ostream &out, Ray const& r) {
    out << "Ray: origin: " << r.origin.x << " " << r.origin.y << " " << r.origin.z;
    out << ", direction: " << r.direction.x << " " << r.direction.y << " " << r.direction.z;
    return out;
}

glm::vec4 Ray::position(float t)
{
    return origin + direction * t;
}
// #include <iostream>
Ray Ray::transform(glm::mat4& m) {
    // std::cout << *this << std::endl;
    // Ray ret(m * this->direction, m * this->origin);
    // std::cout << ret << std::endl;
    // return ret;
    return Ray(m * this->origin, m * this->direction);
}

