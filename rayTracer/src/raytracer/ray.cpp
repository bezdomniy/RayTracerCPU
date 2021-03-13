#include "ray.h"

Ray::Ray() {}

Ray::Ray(Vec4 origin, Vec4 direction)
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

Vec4 Ray::position(Float t)
{
  return origin + direction * t;
}

// This is faster, but will be slow again if taken out into Geometry namespace... not sure why
inline Float vectors_dot_prod2(const Vec4 &x, const Vec4 &y)
{
  // Float res = 0.0;
  // int i = 0;
  // for (; i <= n - 4; i += 4)
  // for (; i < 4; i++)
  // {
  Float res = (x[0] * y[0] +
                x[1] * y[1] +
                x[2] * y[2] +
                x[3] * y[3]);
  // }
  // for (int i = 0; i < 4; i++)
  // for (; i < n; i++)
  // {
  //   res += x[i] * y[i];
  // }
  return res;
}

Vec4 matrix_vector_mult(const Mat4 &m, const Vec4 &vec)
{ // in matrix form: result = mat * vec;
  // Vec4 result;
  // int i;
  // for (i = 0; i < 4; i++)
  // {
  //   const auto &col = glm::row(m, i);
  //   result[i] = vectors_dot_prod2(col, vec);
  // }
  // return result;

  const auto &row0 = glm::row(m, 0);
  const auto &row1 = glm::row(m, 1);
  const auto &row2 = glm::row(m, 2);
  const auto &row3 = glm::row(m, 3);

  Float x = (row0.x * vec.x +
              row0.y * vec.y +
              row0.z * vec.z +
              row0.w * vec.w);

  Float y = (row1.x * vec.x +
              row1.y * vec.y +
              row1.z * vec.z +
              row1.w * vec.w);

  Float z = (row2.x * vec.x +
              row2.y * vec.y +
              row2.z * vec.z +
              row2.w * vec.w);

  Float w = (row3.x * vec.x +
              row3.y * vec.y +
              row3.z * vec.z +
              row3.w * vec.w);

  return Vec4(x, y, z, w);
}

Vec4 matVecPoint(const Mat4 &m, const Vec4 &vec)
{
  const auto &row0 = glm::row(m, 0);
  const auto &row1 = glm::row(m, 1);
  const auto &row2 = glm::row(m, 2);
  // const auto &row3 = glm::row(m, 3);

  Float x = (row0.x * vec.x +
              row0.y * vec.y +
              row0.z * vec.z +
              row0.w * vec.w);

  Float y = (row1.x * vec.x +
              row1.y * vec.y +
              row1.z * vec.z +
              row1.w * vec.w);

  Float z = (row2.x * vec.x +
              row2.y * vec.y +
              row2.z * vec.z +
              row2.w * vec.w);

  // Float w = (row3.x * vec.x +
  //             row3.y * vec.y +
  //             row3.z * vec.z +
  //             row3.w * vec.w);

  return Vec4(x, y, z, 1.0);
}

Vec4 matVecVector(const Mat4 &m, const Vec4 &vec)
{

  const auto &row0 = glm::row(m, 0);
  const auto &row1 = glm::row(m, 1);
  const auto &row2 = glm::row(m, 2);
  // const auto &row3 = glm::row(m, 3);

  Float x = (row0.x * vec.x +
              row0.y * vec.y +
              row0.z * vec.z);

  Float y = (row1.x * vec.x +
              row1.y * vec.y +
              row1.z * vec.z);

  Float z = (row2.x * vec.x +
              row2.y * vec.y +
              row2.z * vec.z);

  return Vec4(x, y, z, 0.0);
}

// Ray Ray::transform(Mat4 &m)
// {
//   return Ray(matrix_vector_mult(m, this->origin),
//              matrix_vector_mult(m, this->direction));
// }

// Ray Ray::transform(Mat4 &m)
// {
//   return Ray(Geometry::matVecMult(m, this->origin),
//              Geometry::matVecMult(m, this->direction));
// }

Ray Ray::transform(Mat4 &m)
{
  return Ray(m * this->origin, m * this->direction);
}

Ray Ray::transform(Mat4 &m, Vec4 newOrigin)
{
  return Ray(newOrigin, m * this->direction);
}

void Ray::transformInPlace(Mat4 &m)
{
  this->origin = m * this->origin;
  this->direction = m * this->direction;
}