#include <glad/include/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include "../include/RenderSystem.h"



//void ScrollCallback2(const GLFWWindow::EMouseScrolled &mse) {
//  if (cam->fov >= 1.0f && cam->fov <= 70.0f)
//    cam->fov -= (float)mse.offset.y;
//  if (cam->fov <= 1.0f)
//    cam->fov = 1.0f;
//  if (cam->fov >= 70.0f)
//    cam->fov = 70.0f;
//}

// ---------------------------------------------------------------------------------------

WindowManager::WindowManager()
{
  //window.On( ScrollCallback2 );
}

void WindowManager::Init(World& world)
{
	//Create The Game Window and set the Window Title
	//TODO: DX and other Window options , make the window title friendlier to set
	Jellyfish::g_singleton_window = nullptr;
	Jellyfish::g_singleton_window = new Jellyfish::GLWindow;
	pWindow = Jellyfish::g_singleton_window;
	Jellyfish::g_singleton_window->CreateGameWindow((unsigned)m_windowSizeSetting.x, (unsigned)m_windowSizeSetting.y, m_windowFullscreenSetting, "Welcome to Pilcrow Engine v0.0 ft. Jellyfish Renderer! :)");
	
	//Rebroadcast window events out to the engine
	pWindow->On([&](const Jellyfish::GLWindow::EWindowResized &event)
	{
	 world.Emit(event);
	});

	pWindow->On([&](const Jellyfish::GLWindow::EWindowStateChanged &event)
	{
	 world.Emit(event);
	});

	//world emits a keypressed event..
	//maybe use this or move camera manager into engine
	pWindow->On([&](const Jellyfish::GLWindow::EKeyPressed &event)
	{
	 world.Emit(event);
	});

	pWindow->On([&](const Jellyfish::iWindow::EMouseMoved &event)
	{
		world.Emit(event);
	});

	this->On([&](const Jellyfish::iCamera::ECameraLook &event)
	{
		world.Emit(event);
	});

	this->On([&](const Jellyfish::iCamera::ECameraMoved &event)
	{
		world.Emit(event);
	});

	//Set callback functions
	//pWindow->On(MouseCallback2);
  
}

void WindowManager::FrameStart( ) 
{
    this->ProcessInput(); 
    pWindow->FrameStart();
}

void WindowManager::FrameEnd( ) 
{ 
	pWindow->FrameEnd();
}

inline void WindowManager::ProcessInput()
{
	std::vector<int> keyarray;
	pWindow->PollInput(keyarray);

	for (int i = 0; i < keyarray.size(); ++i)
	{
		//TODO: Move GL Code into Jellyfish as low-level interface to render modes

		//TODO: Broadcast keys to input system to map keys to actions
		  //TODO: Input System

		if (keyarray[i] == GLFW_KEY_ESCAPE) 
		{
			pWindow->SetWindowState(Jellyfish::WindowState::closed);
		}
		else if (keyarray[i] == GLFW_KEY_1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (keyarray[i] == GLFW_KEY_2) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.5f);
		}
		else if (keyarray[i] == GLFW_KEY_3)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		else if (keyarray[i] == GLFW_KEY_W)
		{
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 0.f, 0.f, 1.f };
			event.offset *= Dt;
			this->Emit(event);
		}
		else if (keyarray[i] == GLFW_KEY_S)
		{
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 0.f, 0.f, -1.f };
			event.offset *= Dt;
			this->Emit(event);
		}
		else if (keyarray[i] == GLFW_KEY_A)
		{
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { -1.f, 0.f, 0.f };
			event.offset *= Dt;
			this->Emit(event); 
		}
		else if (keyarray[i] == GLFW_KEY_D)
		{
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 1.f, 0.f, 0.f };
			event.offset *= Dt;
			this->Emit(event); 
		}
		else if (keyarray[i] == GLFW_KEY_SPACE) 
		{
			//center camera target on origin
			Jellyfish::iCamera::ECameraLook event;
			event.target = { 0.f, 0.f, 0.f };
			this->Emit(event); 
		}
		
	}//endfunc
}


// ---------------------------------------------------------------------------------------

void CameraManager::Init(World& world)
{
	//Event Listening and actions
	world.On([&](const Jellyfish::iCamera::ECameraCreated &event)
	{
		RegisterCamera(event.pCamera);
	});

	world.On([&](const Jellyfish::iCamera::ECameraLook &event)
	{
		//change lookat
		m_currentCamera->LookAt(event.target);
	});

	world.On([&](const Jellyfish::iCamera::ECameraMoved &event)
	{
		//reposition the camera
		m_currentCamera->Move(event.offset);
	});

	world.On([&](const Jellyfish::iWindow::EMouseMoved &event)
	{
		if (event.cursorData.buttonHeld[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS)
		{
			float sensitivity{ .07f };
			const float xoff{ sensitivity * (float)(event.newPosition.x - event.oldPosition.x) };
			const float yoff{ sensitivity * (float)(event.newPosition.y - event.oldPosition.y) };
			
			float newYaw = m_currentCamera->GetYaw() + xoff;
			float newPitch = m_currentCamera->GetPitch() - yoff;

			m_currentCamera->SetPitch(newPitch);
			m_currentCamera->SetYaw(newYaw);
		}
	});

	//Rebroadcast CameraManager events out to the engine
	this->On([&](const ECurrentCamChanged &event)
	{
		world.Emit(event);
	});

	
}

void CameraManager::Update(float dt)
{
	m_currentCamera->Update(dt);
}

glm::mat4 CameraManager::View() const
{
	return m_currentCamera->GetView();
}

glm::mat4 CameraManager::Projection() const
{
	return m_currentCamera->GetProjection();
}

void CameraManager::RegisterCamera(std::shared_ptr<Jellyfish::iCamera> camera, std::string id)
{
	//if a string id was not supplied, we'll use a number
	if (id == "")
	{
		id = std::to_string(this->idCounter++);
	}

	m_cameras[id] = camera;

	//if no camera has been set as current, set this one
	if (m_currentCamera == nullptr)
	{
		m_currentCamera = camera;

		ECurrentCamChanged event;
		event.pNewCamera = m_currentCamera;
		this->Emit(event);
	}
		
	return;
}

void CameraManager::SetCameraByID(std::string id)
{
	m_currentCamera = m_cameras[id];

	ECurrentCamChanged event;
	event.pNewCamera = m_currentCamera;
	this->Emit(event);
}

std::shared_ptr<Jellyfish::iCamera> CameraManager::GetCurrentCamera() const
{
	return m_currentCamera;
}
