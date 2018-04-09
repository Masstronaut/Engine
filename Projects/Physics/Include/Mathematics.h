#pragma once

#include <glm/glm.hpp>

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::ivec2 Vec2i;
typedef glm::ivec3 Vec3i;

typedef glm::mat2 Mat2;
typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;
typedef glm::quat Quat;
typedef glm::mat4x3 Trans; // Affine Mat4

typedef const Vec2& Vec2Param;
typedef const Vec3& Vec3Param;
typedef const Vec4& Vec4Param;

typedef const Vec2i& Vec2iParam;
typedef const Vec3i& Vec3iParam;

typedef const Mat2& Mat2Param;
typedef const Mat3& Mat3Param;
typedef const Mat4& Mat4Param;
typedef const Quat& QuatParam;
typedef const Trans& TransParam;

const int Epsilon = std::numeric_limits<float>::epsilon();
const int Infinity = std::numeric_limits<float>::infinity();