// #include "geometry.h"

// inline Float Geometry::vecDot(const Vec4 &x, const Vec4 &y)
// {
//     Float res = 0.0;
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

// Vec4 Geometry::matVecMult(const Mat4 &m, const Vec4 &vec)
// { // in matrix form: result = mat * vec;
//     Vec4 result;
//     int i;
//     for (i = 0; i < 4; i++)
//     {
//         const auto &col = glm::row(m, i);
//         result[i] = vecDot(col, vec);
//     }
//     return result;
// }