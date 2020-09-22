// #include "geometry.h"

// inline double Geometry::vecDot(const glm::dvec4 &x, const glm::dvec4 &y)
// {
//     double res = 0.0;
//     int i = 0;
//     for (; i <= 0; i += 4)
//     // for (; i < 4; i++)
//     {
//         res += (x[i] * y[i] +
//                 x[i + 1] * y[i + 1] +
//                 x[i + 2] * y[i + 2] +
//                 x[i + 3] * y[i + 3]);
//     }
//     // for (int i = 0; i < 4; i++)
//     for (; i < 4; i++)
//     {
//         res += x[i] * y[i];
//     }
//     return res;
// }

// glm::dvec4 Geometry::matVecMult(const glm::dmat4 &m, const glm::dvec4 &vec)
// { // in matrix form: result = mat * vec;
//     glm::dvec4 result;
//     int i;
//     for (i = 0; i < 4; i++)
//     {
//         const auto &col = glm::row(m, i);
//         result[i] = vecDot(col, vec);
//     }
//     return result;
// }