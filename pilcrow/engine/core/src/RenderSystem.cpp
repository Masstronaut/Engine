#include <glad/include/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include "../include/RenderSystem.h"

void MouseCallback2(const Jellyfish::iWindow::EMouseMoved &mme) {
	if (mme.cursorData.buttonHeld[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS)
	{
		float sensitivity{ .07f };
		const float xoff{ sensitivity * (float)(mme.newPosition.x - mme.oldPosition.x) };
		const float yoff{ sensitivity * (float)(mme.newPosition.y - mme.oldPosition.y) };
		//cam->yaw += xoff;
		//cam->pitch -= yoff;
		//if (cam->pitch > 89.9f) cam->pitch = 89.9f;
		//if (cam->pitch < -89.9f) cam->pitch = -89.9f;
    }
}
//void ScrollCallback2(const GLFWWindow::EMouseScrolled &mse) {
//  if (cam->fov >= 1.0f && cam->fov <= 70.0f)
//    cam->fov -= (float)mse.offset.y;
//  if (cam->fov <= 1.0f)
//    cam->fov = 1.0f;
//  if (cam->fov >= 70.0f)
//    cam->fov = 70.0f;
//}


WindowManager::WindowManager()
{
  //window.MouseMode(CursorMode::disabled);

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

	//Set callback functions
	pWindow->On(MouseCallback2);
  
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
			//cam.position += camSpeed * cam.Front();
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 0.f, 0.f, 1.f };
			event.offset *= Dt;
			this->Emit(event);  //see if I need the world to do this..
		}
		else if (keyarray[i] == GLFW_KEY_S)
		{
			//cam.position -= camSpeed * cam.Front();
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 0.f, 0.f, -1.f };
			event.offset *= Dt;
			this->Emit(event);  //see if I need the world to do this..
		}
		else if (keyarray[i] == GLFW_KEY_A)
		{
			//cam.position -= cam.Right() * camSpeed;
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { -1.f, 0.f, 0.f };
			event.offset *= Dt;
			this->Emit(event);  //see if I need the world to do this..
		}
		else if (keyarray[i] == GLFW_KEY_D)
		{
			//cam.position += cam.Right() * camSpeed;
			Jellyfish::iCamera::ECameraMoved event;
			event.offset = { 1.f, 0.f, 0.f };
			event.offset *= Dt;
			this->Emit(event);  //see if I need the world to do this..
		}
		else if (keyarray[i] == GLFW_KEY_SPACE) //center camera on origin
		{
			Jellyfish::iCamera::ECameraLook event;
			event.target = { 0.f, 0.f, 0.f };
			this->Emit(event);  //see if I need the world to do this..
		}
		
	}//endfunc

	

  
}
