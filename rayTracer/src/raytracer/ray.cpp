#include "ray.h"

Ray::Ray() {}

Ray::Ray(glm::dvec4 origin, glm::dvec4 direction)
{
  this->origin = origin;
  this->direction = direction;
}

Ray::~Ray() {}

std::ostream &operator<<(std::ostream &out, Ray const &r)
{
  out << "Ray: origin: " << r.origin.x << " " << r.origin.y << " "
      << r.origin.z;
  out << ", direction: " << r.direction.x << " " << r.direction.y << " "
      << r.direction.z;
  return out;
}

glm::dvec4 Ray::position(double t)
{
  return origin + direction * t;
}

// This is faster, but will be slow again if taken out into Geometry namespace... not sure why
inline double vectors_dot_prod2(const glm::dvec4 &x, const glm::dvec4 &y, int n)
{
  double res = 0.0;
  int i = 0;
  for (; i <= n - 4; i += 4)
  // for (; i < 4; i++)
  {
    res += (x[i] * y[i] +
            x[i + 1] * y[i + 1] +
            x[i + 2] * y[i + 2] +
            x[i + 3] * y[i + 3]);
  }
  // for (int i = 0; i < 4; i++)
  for (; i < n; i++)
  {
    res += x[i] * y[i];
  }
  return res;
}

glm::dvec4 matrix_vector_mult2(const glm::dmat4 &m, const glm::dvec4 &vec, int rows, int cols)
{ // in matrix form: result = mat * vec;
  glm::dvec4 result;
  int i;
  for (i = 0; i < rows; i++)
  {
    const auto &col = glm::row(m, i);
    result[i] = vectors_dot_prod2(col, vec, cols);
  }
  return result;
}

Ray Ray::transform(glm::dmat4 &m)
{
  return Ray(matrix_vector_mult2(m, this->origin,
                                 4, 4),
             matrix_vector_mult2(m, this->direction,
                                 4, 4));
}

// Ray Ray::transform(glm::dmat4 &m)
// {
//   return Ray(Geometry::matVecMult(m, this->origin),
//              Geometry::matVecMult(m, this->direction));
// }

// Ray Ray::transform(glm::dmat4 &m)
// {
//   return Ray(m * this->origin, m * this->direction);
// }

Ray Ray::transform(glm::dmat4 &m, glm::dvec4 newOrigin)
{
  return Ray(newOrigin, m * this->direction);
}

void Ray::transformInPlace(glm::dmat4 &m)
{
  this->origin = m * this->origin;
  this->direction = m * this->direction;
}