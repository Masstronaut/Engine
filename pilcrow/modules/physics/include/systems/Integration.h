#pragma once

#include <entity/EntitiesWith.hpp>

#include "../components/Body.h"
#include <components/Transform.h>

class Integration {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};
