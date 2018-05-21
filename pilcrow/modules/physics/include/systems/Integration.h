#pragma once

#include <entity/EntitiesWith.hpp>

#include "../Components/Body.h"
#include <components/Transform.h>

class Integration {
public:
  EntitiesWith<Transform, Body> Entities;

  void Update(float dt);
};