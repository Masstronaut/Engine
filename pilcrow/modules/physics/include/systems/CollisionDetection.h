#pragma once

#include <entity/EntitiesWith.hpp>

#include "../../include/components/Body.h"
#include <components/Transform.h>

class CollisionDetection {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};