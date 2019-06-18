#pragma once

#include "pilcrow/engine/core/entity/EntitiesWith.hpp"

#include "pilcrow/modules/physics/components/Body.h"
#include "pilcrow/engine/core/components/Transform.h"

class Integration {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};
