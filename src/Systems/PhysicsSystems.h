#pragma once

#include "../Entity/EntitiesWith.hpp"

#include "../Components/Transform.h"
#include "../Components/PhysicsBody.h"

class PhysicsIntegration
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;
	void Update(float dt);
	void Process(Transform &transform, PhysicsBody& body);
	
private:
	float Dt;

	void UpdateVelocities(PhysicsBody& body);
	void ApplyGravity(PhysicsBody& body);
	void Integrate(Transform &transform, const PhysicsBody& body);
};

class PhysicsCollisionDetection
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;
	// TODO Should be < const Transform, (optional) const PhysicsBody, const IPhysicsCollider >
	// TODO EntitiesWith<Manifold> CollidingEntities;
	void Update(float dt);
};

class PhysicsResolution
{
public:
	EntitiesWith<Transform, PhysicsBody> Entities;

	void Update(float dt);
};