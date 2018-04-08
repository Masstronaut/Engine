#pragma once
#include <glm/glm.hpp>

struct PhysicsBody 
{
	float massInverse = 1.0f;
	glm::mat3 inertiaTensor = glm::mat3(1.0f);
	
	float restitution = 0.0f;

	glm::vec3 velocity{ 0.f, 0.f, 0.f };
	glm::vec3 angularVelocity{ 0.f, 0.f, 0.f };

	glm::vec3 impulse{ 0.f, 0.f, 0.f };
	glm::vec3 angularImpulse{ 0.f, 0.f, 0.f };

	// float GetMass();
	// void SetMass(float mass);
};