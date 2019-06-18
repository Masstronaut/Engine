#pragma once

#include <entity/EntitiesWith.hpp>

#include "../components/Body.h"
#include <components/Transform.h>

class CollisionDetection {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};