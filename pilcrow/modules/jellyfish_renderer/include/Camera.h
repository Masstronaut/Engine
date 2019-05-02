#pragma once

//std lib
#include <unordered_map> //multiple cams are stored in a map with string names
#include <string>

//glm math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//ours
#include <utils/include/EventArena.hpp> //event messaging

namespace Jellyfish
{
	class iCamera : public EventArena
	{
	public:

//not sure if I want this yet but trying it out
#pragma region EventTypes
		struct ECameraCreated 
		{
			iCamera* pCamera = nullptr;
		};

		struct ECameraMoved
		{
			//z = front , y= up, x = right for all positive vals
			glm::vec3 offset = { 0.f, 0.f, 0.f };
		};

		struct ECameraLook
		{
			glm::vec3 target = { 0.f, 0.f, 0.f };
		};
#pragma endregion

		virtual ~iCamera() {}
		virtual void Update(float dt) = 0;

		glm::mat4 GetView() const
		{
			return m_view;
		}

		glm::mat4 GetProjection() const
		{
			return m_projection;
		}

		void SetPerspectiveProjection(float window_size_x, float window_size_y)
		{
			m_projection = glm::perspective(glm::radians(m_fov), window_size_x / window_size_y, m_nearplane, m_farplane);
		}

		void SetOrthoProjection(float window_size_x, float window_size_y)
		{
			m_projection = glm::ortho(0.f, window_size_x, 0.f, window_size_y);
		}

		void SetPosition(glm::vec3 pos)
		{
			m_position = pos;
		}
		
		void SetPosition(glm::vec3 pos)
		{
			m_position = pos;
		}
		
		void SetPitch(float pitch)
		{
			m_pitch = pitch;
		}

		void SetYaw(float yaw)
		{
			m_yaw = yaw;
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

		glm::mat4 View() const
		{
			return glm::lookAt(m_position, m_position + Front(), m_up);
		}

		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::vec3 m_position{ 0.f, 2.f, 0.f };
		glm::vec3 m_up{ 0.f, 1.f, 0.f };
		glm::vec3 m_right{ 1.f, 0.f, 0.f };
		
		float m_fov{ 80.f };
		float m_nearplane{ 0.1f };
		float m_farplane{ 100.f };
		float m_yaw{ 0.f };
		float m_pitch{ 0.f };

		float m_speed{ 1.0f };
	};
	
	class CameraManager : public EventArena
	{
	public:

		void Init()
		{

		}

		void RegisterCamera(iCamera* camera, std::string id = "")
		{ 
			//if a string id was not supplied, we'll use a number
			if (id == "")
			{
				id = std::to_string(this->idCounter++);
			}

			m_cameras[id] = camera; 

			//if no camera has been set as current, set this one
			if (currentCamera == nullptr)
				currentCamera = camera;

			return;
		}
		
		void SetCameraByID(std::string id) 
		{ 
			currentCamera = m_cameras[id]; 
		}

		//TODO: const correctness
		iCamera* GetCurrentCamera()
		{
			return currentCamera;
		}

		void Update(float dt) 
		{ 
			currentCamera->Update(dt); 
		}
		
		glm::mat4 View()
		{ 
			return currentCamera->GetView();
		}

		glm::mat4 Projection()
		{
			return currentCamera->GetProjection();
		}
	
	private:
		 std::unordered_map<std::string, iCamera*> m_cameras;
		 iCamera* currentCamera = nullptr;
		 int idCounter = 0;
	};

	// ------------------------------------
	// CAMERA TYPES AND IMPLEMENTATIONS
	// ------------------------------------

	class Camera : public iCamera //follows player // TODO:behavior
	{
	public:
		Camera();
		void Update(float dt);
	}; //end class Camera

	class DebugCamera : public iCamera //free flying camera //TODO:behavior
	{
	public:
		DebugCamera();
		void Update(float dt);
	}; //end class DebugCamera


} //end namespace

