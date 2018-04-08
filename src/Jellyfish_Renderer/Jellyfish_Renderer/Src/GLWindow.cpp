#pragma once
#include <glad/glad.h> //opengl
#include <GLFW/glfw3.h> //glfw window stuff
//#define GLEW_STATIC
//#include <GL/glew.h> //glew
#include <iostream> //cout
#include "../Include/GLWindow.h" 

//TODO: fix, put in settings or something
const unsigned glmajor = 4;
const unsigned glminor = 5;


namespace Jellyfish
{
	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(m_WindowHandle);
		m_WindowHandle = nullptr;
		glfwTerminate();
	}

	void GLWindow::CreateGameWindow(unsigned width, unsigned height, bool fullscreen)
	{
		if (glfwInit()) 
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glmajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glminor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
				m_WindowHandle = glfwCreateWindow(tempWidth, tempHeight, this->GetWindowTitle().c_str(), primaryMonitor, NULL);
			}
			else
			{
				//Create a restored resizable window at specified resolution
				glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
				m_Size = glm::uvec2(width, height);
				m_WindowHandle = glfwCreateWindow(width, height, this->GetWindowTitle().c_str(), NULL, NULL);
			}
				

			if (!m_WindowHandle) {
				std::cout << "Failed to create window! Bad Handle." << std::endl;
				system("pause");
				return;
			}
			
			//Set Context
			glfwMakeContextCurrent(m_WindowHandle);

			//Set Callback Functions
			glfwSetFramebufferSizeCallback(m_WindowHandle, ResizeWindow);
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

	void GLWindow::ResizeWindow(GLFWwindow* windowhandle, int width, int height)
	{

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

	}
	
	void GLWindow::SetWindowSize(const glm::uvec2 &size)
	{
		glfwMakeContextCurrent(m_WindowHandle);
		glViewport(0, 0, size.x, size.y);
	}
	void GLWindow::SetWindowTitle(const std::string &title)
	{
		glfwSetWindowTitle(m_WindowHandle, title.c_str());
	}
	void GLWindow::SetWindowState(WindowState state)
	{
		switch (state) 
		{
			case WindowState::maximized:
				glfwMaximizeWindow(m_WindowHandle);
				break;
			case WindowState::minimized:
				glfwIconifyWindow(m_WindowHandle);
				break;
			case WindowState::restored:
				glfwRestoreWindow(m_WindowHandle);
				break;
			case WindowState::closed:
				glfwSetWindowShouldClose(m_WindowHandle, true);
				break;
			default:
				return;
		}
		m_State = state;
	}

		
} //namespace Jellyfish
