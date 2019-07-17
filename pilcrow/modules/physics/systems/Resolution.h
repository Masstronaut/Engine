#pragma once

#include "pilcrow/engine/core/entity/EntitiesWith.hpp"

#include "pilcrow/engine/core/components/Transform.h"

#include "pilcrow/modules/physics/components/Body.h"

class Resolution {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};
