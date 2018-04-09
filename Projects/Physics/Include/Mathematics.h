#pragma once

#include <glm/glm.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;

using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Quat = glm::quat;
using Trans = glm::mat4x3; // Affine Mat4

using Vec2Param = const Vec2&;
using Vec3Param = const Vec3&;
using Vec4Param = const Vec4&;

using Vec2iParam = const Vec2i&;
using Vec3iParam = const Vec3i&;

using Mat2Param = const Mat2&;
using Mat3Param = const Mat3&;
using Mat4Param = const Mat4&;
using QuatParam = const Quat&;
using TransParam = const Trans&;

const float Epsilon = std::numeric_limits<float>::epsilon();
const float Infinity = std::numeric_limits<float>::infinity();