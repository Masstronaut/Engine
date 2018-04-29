#pragma once

#include <entity/EntitiesWith.hpp>

#include <components/Transform.h>
#include "../Components/Body.h"

class Integration
{
public:
	EntitiesWith<Transform, Body> Entities;

	void Update(float dt);

};