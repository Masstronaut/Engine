#pragma once

#include <entity/EntitiesWith.hpp>

#include <components/Transform.h>
#include "../../include/Components/Body.h"

class Resolution
{
public:
	EntitiesWith<Transform, Body> Entities;

	void Update(float dt);
};