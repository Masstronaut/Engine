#pragma once

//glm math
#include <glm/glm.hpp>

//Plane

class Frustum
{
#ifdef vec3
  #undef vec3
#endif
	typedef glm::vec3 vec3;

public:
	Frustum();
	bool Inside(const vec3 &point) const;
	bool Inside(const vec3 &point, const float radius) const;

	const Plane &Get(Side side) 
	{ 
		return m_Planes[side]; 
	}
	
	void SetFOV(float fov) 
	{
		m_Fov = fov; 
		Init(m_Fov, m_Aspect, m_Near, m_Far); 
	}
	
	void SetAspect(float aspect)
	{
		m_Aspect = aspect; 
		Init(m_Fov, m_Aspect, m_Near, m_Far);
	}
	
	void SetNear(float nearClip)
	{
		m_Near = nearClip; 
		Init(m_Fov, m_Aspect, m_Near, m_Far);
	}
	
	void SetFar(float farClip)
	{
		m_Far = farClip; 
		Init(m_Fov, m_Aspect, m_Near, m_Far);
	}
	
	void Init(const float fov, const float aspect,
		const float near, const float far);
	
	void Render();

private:
	enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };
	Plane m_Planes[NumPlanes]; // planes of the frusum in camera space
	vec3 m_NearClip[4]; // verts of the near clip plane in camera space
	vec3 m_FarClip[4]; // verts of the far clip plane in camera space
	float m_Fov; // field of view in radians
	float m_Aspect; // aspect ratio - width divided by height
	float m_Near; // near clipping distance
	float m_Far; // far clipping distance
};


Frustum::Frustum()
{
	m_Fov = D3DX_PI / 4.0f; // default field of view is 90 degrees
	m_Aspect = 1.0f; // default aspect ratio is 1:1
	m_Near = 1.0f; // default near plane is 1m away from the camera
	m_Far = 1000.0f; // default near plane is 1000m away from the camera
}
bool Frustum::Inside(const vec3 &point) const
{
	for (int i = 0; i<NumPlanes; ++i)
	{
		if (!m_Planes[i].Inside(point))
			return false;
	}
	return true;
}
bool Frustum::Inside(const vec3 &point, const float radius) const
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!m_Planes[i].Inside(point, radius))
			return false;
	}
	// otherwise we are fully in view
	return true;
}

void Frustum::Init(const float fov, const float aspect, const float nearClip, const
	float farClip)
{
	m_Fov = fov;
	m_Aspect = aspect;

	m_Near = nearClip;
	m_Far = farClip;
	double tanFovOver2 = tan(m_Fov / 2.0f);
	vec3 nearRight = (m_Near * tanFovOver2) * m_Aspect * g_Right;
	vec3 farRight = (m_Far * tanFovOver2) * m_Aspect * g_Right;
	vec3 nearUp = (m_Near * tanFovOver2) * g_Up;
	vec3 farUp = (m_Far * tanFovOver2) * g_Up;

	// points start in the upper right and go around clockwise
	m_NearClip[0] = (m_Near * g_Forward) - nearRight + nearUp;
	m_NearClip[1] = (m_Near * g_Forward) + nearRight + nearUp;
	m_NearClip[2] = (m_Near * g_Forward) + nearRight - nearUp;
	m_NearClip[3] = (m_Near * g_Forward) - nearRight - nearUp;
	m_FarClip[0] = (m_Far * g_Forward) - farRight + farUp;
	m_FarClip[1] = (m_Far * g_Forward) + farRight + farUp;
	m_FarClip[2] = (m_Far * g_Forward) + farRight - farUp;
	m_FarClip[3] = (m_Far * g_Forward) - farRight - farUp;
	// now we have all eight points. Time to construct six planes.
	// the normals point away from you if you use counter clockwise verts.
	Vec3 origin(0.0f, 0.0f, 0.0f);
	m_Planes[Near].Init(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
	m_Planes[Far].Init(m_FarClip[0], m_FarClip[1], m_FarClip[2]);
	m_Planes[Right].Init(m_FarClip[2], m_FarClip[1], origin);
	m_Planes[Top].Init(m_FarClip[1], m_FarClip[0], origin);
	m_Planes[Left].Init(m_FarClip[0], m_FarClip[3], origin);
	m_Planes[Bottom].Init(m_FarClip[3], m_FarClip[2], origin);
}