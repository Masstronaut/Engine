#pragma once

#include <entity/EntitiesWith.hpp>

#include <components/Transform.h>
#include "../../include/components/Body.h"


class CollisionDetection
{
public:
	EntitiesWith<Transform, Body> Entities;

	void Update(float dt);
};