#pragma once

#include "../Entity/EntitiesWith.hpp"

#include "../Components/Transform.h"
#include "../Components/PhysicsBody.h"

class PhysicsIntegration
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;
	void Update(float dt);
};

class PhysicsCollisionDetection
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;
		// TODO Should be < const Transform, (optional) const PhysicsBody, const IPhysicsCollider >
	void Update(float dt);
};

class PhysicsResolution
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;
	// TODO EntitiesWith<Manifold> CollidingEntities;

	void Update(float dt);
};