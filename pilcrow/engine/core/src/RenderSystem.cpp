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

		//also send to the window to deal with internal keycodes
		pWindow->ProcessMouseEvent(event);
	});

	pWindow->On([&](const Jellyfish::iCamera::ECameraLook &event)
	{
		world.Emit(event);
	});

	pWindow->On([&](const Jellyfish::iCamera::ECameraMoved &event)
	{
		world.Emit(event);
	});
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
	
	//TODO: Use a proper Input Manager for this at some point
	//Map key arrays to actions ans such
	//have defines for which input key codes to use maybe..?
	pWindow->ProcessInput(keyarray, Dt);
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
		m_currentCamera->Look(event);
	});

	world.On([&](const Jellyfish::iCamera::ECameraMoved &event)
	{
		//reposition the camera
		m_currentCamera->Move(event);
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
