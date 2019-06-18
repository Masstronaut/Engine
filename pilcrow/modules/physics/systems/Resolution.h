#pragma once

#include <entity/EntitiesWith.hpp>

#include "../components/Body.h"
#include <components/Transform.h>

class Resolution {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};
