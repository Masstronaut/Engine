#pragma once
#include "../include/Camera.h"

namespace Jellyfish
{
	//Default Camera -- (Placeholder)
	Camera::Camera()
	{
		m_view = View();
	}

	void Camera::Update(float dt)
	{
		m_view = View();
	}

	//Debug / Test Camera
	DebugCamera::DebugCamera()
	{
		m_view = View();
	}

	void DebugCamera::Update(float dt)
	{
		m_view = View();
	}
	

} //end namespace

