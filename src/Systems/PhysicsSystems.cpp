#include "PhysicsSystems.h"

const float EPSILON = std::numeric_limits<float>::epsilon();

#pragma region Integration

void PhysicsIntegration::Update(float dt)
{
	// TODO handle any engine/physics dt magic
	Dt = dt;
}

void PhysicsIntegration::Process(Transform & transform, PhysicsBody & body)
{
	UpdateVelocities(body);
	Integrate(transform, body);
	ApplyGravity(body);
}

void PhysicsIntegration::UpdateVelocities(PhysicsBody & body)
{
	body.velocity += body.massInverse * body.impulse;
	body.impulse = { 0.f, 0.f, 0.f };

	body.angularVelocity += body.inertiaTensor * body.angularImpulse;
	body.angularImpulse = { 0.f, 0.f, 0.f };
}

void PhysicsIntegration::ApplyGravity(PhysicsBody & body)
{
	const glm::vec3 GRAVITY = { 0.f, -5.0f, 0.f }; // m/s^2, approx half earth's

	if (body.massInverse > EPSILON)
	{
		body.impulse += GRAVITY / body.massInverse;
	}
}

void PhysicsIntegration::Integrate(Transform & transform, const PhysicsBody & body)
{
	transform.position += body.velocity * Dt;

	// TODO 3D Rotation
}

#pragma endregion

#pragma region Collision

void PhysicsCollisionDetection::Update(float dt)
{

}

#pragma endregion

#pragma region Resolution

void PhysicsResolution::Update(float dt)
{

}

#pragma endregion
