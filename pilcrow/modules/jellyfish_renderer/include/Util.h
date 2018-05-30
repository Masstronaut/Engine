#pragma once

//std
#include <string>

//ours
#include "Utils/include/ResourceSettings.h"

namespace Jellyfish
{
  static std::string ShaderPath()
  {
	  return std::string(g_ResourcePath) + "Shaders/";
  }
}