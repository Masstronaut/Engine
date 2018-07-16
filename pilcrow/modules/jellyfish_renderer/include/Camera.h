#pragma once

//glm math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//multiple cams are stored in a map with string names
#include <unordered_map> 
#include <string>

namespace Jellyfish
{
	class iCamera
	{
	public:
		virtual void Update(float dt) = 0;

		glm::mat4 SetView()
		{
			m_view = glm::lookAt(m_position, m_position + Front(), m_up);
		}

		glm::mat4 GetView() const
		{
			return m_view;
		}
	
	private:
		glm::vec3 Front() const
		{
			return glm::normalize(
				glm::vec3
				{
					cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw)),
					sin(glm::radians(m_pitch)),
					cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw))
				});
		}

		glm::vec3 Right() const
		{
			return glm::normalize(glm::cross(Front(), m_up));
		}

		glm::mat4 LookAt(glm::vec3 target)
		{
			m_right = Right();

			return glm::mat4
			{
				m_right.x,  m_right.y,  m_right.z,  0,
				m_up.x,     m_up.y,     m_up.z,     0,
				target.x, target.y, target.z, 0,
				m_position.x, m_position.y, m_position.z, 1.f
			};
		}

		glm::vec3 Direction() const
		{
			return glm::normalize(m_position - Front());
		}

		glm::mat4 m_view;
		glm::vec3 m_position{ 0.f, 0.f, 0.f };
		glm::vec3 m_up{ 0.f, 1.f, 0.f };
		glm::vec3 m_right{ 1.f, 0.f, 0.f };
		
		float m_fov{ 80.f };
		float m_nearplane{ 0.1f };
		float m_farplane{ 100.f };
		float m_yaw{ 0.f };
		float m_pitch{ 0.f };
	};
	
	class CameraManager
	{
	public:
		void RegisterCamera(std::string id, iCamera* camera) 
		{ 
			m_cameras[id] = camera; 
		}
		
		void SetCamera(std::string id) 
		{ 
			currentCamera = m_cameras[id]; 
		}

		void Update(float dt) 
		{ 
			currentCamera->Update(dt); 
		}
		
		glm::mat4 View()
		{ 
			return currentCamera->GetView();
		}
	
	private:
		 std::unordered_map<std::string, iCamera*> m_cameras;
		 iCamera* currentCamera;
	};

	// ------------------------------------
	// CAMERA TYPES AND IMPLEMENTATIONS
	// ------------------------------------

	class PlayerCamera3D : public iCamera //follows player
	{
	public:
		void Update(float dt);
	}; //end class Camera

	class PlayerCamera2D : public iCamera //follows player on XY only
	{
	public:
		void Update(float dt);
	}; //end class Camera

	class DebugCamera3D : public iCamera //free camera
	{
	public:
		void Update(float dt);
	}; //end class Camera

} //end namespace

