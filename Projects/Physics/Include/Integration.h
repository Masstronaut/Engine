#pragma once

#include "Entity/EntitiesWith.hpp"

#include <Components\Transform.h>
#include <Body.h>

class Integration
{
	EntitiesWith<Transform, Body> Entities;

	void Update(float dt);

};