#pragma once
// #define GLM_FORCE_INTRINSICS
// #define GLM_FORCE_ALIGNED_GENTYPES
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

// #define GLM_FORCE_PURE

// #include "mathfu/vector.h"
// #include "mathfu/matrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef double Float;

// typedef mathfu::Matrix<Float, 4> Mat4;
// typedef mathfu::Vector<Float, 4> Vec4;
// typedef mathfu::Vector<Float, 3> Vec3;
// typedef mathfu::Vector<Float, 2> Vec2;

typedef glm::dmat4 Mat4;
typedef glm::dvec4 Vec4;
typedef glm::dvec3 Vec3;
typedef glm::dvec2 Vec2;

// template <typename T>
// constexpr auto rotate = glm::rotate<T>;

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF)                                       \
    template <typename... Args>                                                              \
    inline auto highLevelF(Args &&...args)->decltype(lowLevelF(std::forward<Args>(args)...)) \
    {                                                                                        \
        return lowLevelF(std::forward<Args>(args)...);                                       \
    }

ALIAS_TEMPLATE_FUNCTION(rotate, glm::rotate);

// glm::rotate
// glm::lookAt
// glm::affineInverse
// glm::normalize
// glm::scale
// glm::translate
// glm::transpose
// glm::length
// glm::cross
// glm::atan
// glm::acos
// glm::clamp
// glm::ballRand
// glm::pi
// glm::dot
// glm::reflect
// glm::min
// glm::max
// glm::ivec3
