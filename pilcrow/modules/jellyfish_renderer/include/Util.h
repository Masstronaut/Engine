#pragma once

#include <string>

#include "Utils/include/ResourceSettings.h"

namespace Jellyfish
{
  static std::string ShaderPath()
  {
	  return std::string(g_ResourcePath) + "Shaders/";
  }
}