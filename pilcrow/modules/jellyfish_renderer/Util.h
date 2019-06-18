#pragma once

// std
#include <string>

// ours
#include "utils/include/ResourceSettings.h"

namespace Jellyfish {
static std::string ShaderPath() {
  return std::string(g_ResourcePath) + "Shaders/";
}
}  // namespace Jellyfish
