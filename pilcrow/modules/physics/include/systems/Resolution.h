#pragma once

#include <entity/EntitiesWith.hpp>

#include "../../include/Components/Body.h"
#include <components/Transform.h>

class Resolution {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};