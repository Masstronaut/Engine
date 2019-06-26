#pragma once

// std
#include <string>

// ours
#include "Utils/ResourceSettings.hpp"

namespace Jellyfish {
static std::string ShaderPath() {
  return std::string(g_ResourcePath) + "Shaders/";
}
}  // namespace Jellyfish
