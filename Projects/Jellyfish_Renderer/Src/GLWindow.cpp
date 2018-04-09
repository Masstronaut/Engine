#pragma once
#include <glad/glad.h> //opengl
#include <GLFW/glfw3.h> //glfw window stuff
//#define GLEW_STATIC
//#include <GL/glew.h> //glew
#include <iostream> //cout
#include "../Include/GLWindow.h" 



//TODO: fix, put in settings or something
const unsigned glmajor = 4;
const unsigned glminor = 3;


namespace Jellyfish
{
	GLWindow::GLWindow()
	{
		g_singleton_window = this;
	}

	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(m_WindowHandle);
		m_WindowHandle = nullptr;
		glfwTerminate();
	}

	void GLWindow::CreateGameWindow(unsigned width, unsigned height, bool fullscreen, std::string title)
	{
		if (glfwInit()) 
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glmajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glminor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
#if defined(__APPLE__)
			// required for MAC OSX support
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

			//Create the window and get a pointer to the handle
			if (fullscreen)
			{
				//Get Monitor Resolution
				GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* primaryMonMode = glfwGetVideoMode(primaryMonitor);
				int tempWidth = primaryMonMode->width;
				int tempHeight = primaryMonMode->height;

				//Create a borderless fullscreen window at current resolution
				m_Size = glm::uvec2(tempWidth, tempHeight);
				m_WindowHandle = glfwCreateWindow(tempWidth, tempHeight, title.c_str(), primaryMonitor, NULL);
			}
			else
			{
				//Create a restored resizable window at specified resolution
				glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
				m_Size = glm::uvec2(width, height);
				m_WindowHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			}
				

			if (!m_WindowHandle) {
				std::cout << "Failed to create window! Bad Handle." << std::endl;
				system("pause");
				return;
			}
			
			//Set Context
			glfwMakeContextCurrent(m_WindowHandle);

			//Set Callback Functions
			glfwSetFramebufferSizeCallback(m_WindowHandle, Callback_ResizeWindow);
			glfwSetWindowCloseCallback(m_WindowHandle, Callback_WindowClose);
			glfwSetWindowPosCallback(m_WindowHandle, Callback_WindowMove);
			//glfwSetKeyCallback(window, key_callback);
			//glfwSetCursorPosCallback(window, mouse_callback);
			//glfwSetScrollCallback(window, scroll_callback);

			// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
			//glewExperimental = GL_TRUE;
			// Initialize GLEW to setup the OpenGL Function pointers
			//if (glewInit() != GLEW_OK)
			//{
			//	std::cout << "Failed to initialize GLEW" << std::endl;
			//	system("pause");
			//	return;
			//}


			// Initialize GLAD to setup the OpenGL Function pointers
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
			{
				std::cout << "Failed to initialize GLAD Loader!" << std::endl;
				system("pause");
				return;
			}

			// Define the viewport dimensions
			GLsizei w = this->GetWindowWidth();
			GLsizei h = this->GetWindowHeight();
			glfwGetFramebufferSize(m_WindowHandle, &w, &h);
			glViewport(0, 0, w, h);

		}
		else
		{
			std::cout << "Failed to initialize GLFW!" << std::endl;
			system("pause");
			return;
		}
		//ALL OK!
		return;
	}
	void GLWindow::UpdateGameWindow()
	{

	}
	void GLWindow::DisplayGameWindow()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	bool GLWindow::IsOpen()
	{
		return m_Open;
	}
	bool GLWindow::IsActive()
	{
		return m_Active;
	}
	void GLWindow::setActive(bool active)
	{

	}

	void GLWindow::ResizeWindow(unsigned width, unsigned height)
	{
		if ((width != this->m_Size.x) || (height != this->m_Size.y))
		{
			//send off event to notify other systems
			EWindowResized event;
			event.newSize = glm::uvec2{ width, height };
			event.oldSize = m_Size;
			g_singleton_window->Emit(event);

			m_Size.x = width;
			m_Size.y = height;
			std::cout << "Window was resized to: " << width << " by " << height << std::endl;

			glfwMakeContextCurrent(m_WindowHandle);
			glViewport(0, 0, width, height);
		}
	}

	unsigned GLWindow::GetWindowWidth()
	{
		return m_Size.x;
	}
	unsigned GLWindow::GetWindowHeight()
	{
		return m_Size.y;
	}

	void GLWindow::PollEvents()
	{
		glfwPollEvents();
	}
	
	void GLWindow::FrameEnd()
	{
		DisplayGameWindow();
		PollEvents();
	}

	void GLWindow::SetWindowTitle(const std::string &title)
	{
		glfwSetWindowTitle(m_WindowHandle, title.c_str());
	}
	void GLWindow::SetWindowState(WindowState state)
	{
		if (m_State != state)
		{
			WindowState oldState{ m_State };
			EWindowStateChanged event { m_State, state };
			this->Emit(event);

			//TODO: stringify events at compile time for friendly output
			switch (state)
			{
			case WindowState::maximized:
				glfwMaximizeWindow(m_WindowHandle);
				std::cout << "Window state was changed to: " << "maximized" << std::endl;
				break;
			case WindowState::minimized:
				glfwIconifyWindow(m_WindowHandle);
				std::cout << "Window state was changed to: " << "minimized" << std::endl;
				break;
			case WindowState::restored:
				glfwRestoreWindow(m_WindowHandle);
				std::cout << "Window state was changed to: " << "restored" << std::endl;
				break;
			case WindowState::closed:
				glfwSetWindowShouldClose(m_WindowHandle, true);
				std::cout << "Window state was changed to: " << "closed" << std::endl;
				break;
			default:
				return;
			}
			m_State = state;
		}//endif
	}//endfunc

	//Polls Input devices, returns current input in data passed as ref
	void GLWindow::PollInput(std::vector<int>& keyarray) 
	{
		//Keyboard Press
		keyarray.clear(); //clear prev input
		//GLFW key codes range from -1 to 350
		for (int key = -1; key <= 350; ++key)
		{
			if (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS)
			{
				//could use events but array is probably faster
				//EKeyPressed event {key};
				//this->Emit(event);
				keyarray.push_back(key);
				
				//DEBUG OUTPUT
				if ( (key >= 32 && key <= 162) )
				{
					const char* key_name = glfwGetKeyName(key, 0);
					std::cout << "Key Pressed: " << key_name << std::endl;
				}
				else
				{
					std::cout << "Key Pressed: " << key << "(int code)" << std::endl;
				}
				

			}//endif
		}//endfor
	}//endfunc


	void GLWindow::Callback_ResizeWindow(GLFWwindow* windowhandle, int width, int height)
	{
		g_singleton_window->ResizeWindow(width, height);
		return;
	}
	void GLWindow::Callback_WindowClose(GLFWwindow* windowhandle)
	{
		glfwSetWindowShouldClose(windowhandle, true);
		std::cout << "glfw window should close flag was set!" << std::endl;
		return;
	}

	void GLWindow::Callback_WindowMove(GLFWwindow* windowhandle, int xpos, int ypos)
	{
		//DEBUG:
		std::cout << "Window position was moved to: " << xpos << ", " << ypos << std::endl;
	}

} //namespace Jellyfish
