#pragma once
#include "../include/Camera.h"

namespace Jellyfish
{
	//Default Camera -- (Placeholder)
	Camera::Camera()
	{
		ECameraCreated event;
		event.pCamera = this;
		this->Emit(event);
	}

	void Camera::Update(float dt)
	{

	}

	//Debug / Test Camera
	DebugCamera::DebugCamera()
	{
		ECameraCreated event;
		event.pCamera = this;
		this->Emit(event);
	}

	void DebugCamera::Update(float dt)
	{

	}
	

} //end namespace

