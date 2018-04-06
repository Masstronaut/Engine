#pragma once
#include "../Include/GLWindow.h"
#include <glad/glad.h> //opengl
#include <GLFW/glfw3.h> //opengl

//TODO: fix, put in settings or something
const unsigned glmajor = 4;
const unsigned glminor = 5;


namespace Jellyfish
{
	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(m_WindowHandle);
		m_WindowHandle = nullptr;
	}

	void GLWindow::CreateGameWindow(unsigned width, unsigned height, bool fullscreen)
	{
		if (glfwInit()) {
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
				GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* primaryMonMode = glfwGetVideoMode(primaryMonitor);

				//store in a modifyable object for debugging
				//TODO: Improve
				int tempWidth = primaryMonMode->width;
				int tempHeight = primaryMonMode->height;

				m_Size = glm::uvec2(tempWidth, tempHeight);
				m_WindowHandle = glfwCreateWindow(tempWidth, tempHeight, this->GetWindowTitle().c_str(), primaryMonitor, NULL);
				SetWindowSize(m_Size);
			}
			else
				m_WindowHandle = glfwCreateWindow(width, height, this->GetWindowTitle().c_str(), NULL, NULL);


			if (!m_WindowHandle) {
				// @@TODO: Change to real logging
				system("pause");
				exit(-1);
			}
			//m_CallbackHelpers[m_WindowHandle] = this;
			glfwMakeContextCurrent(m_WindowHandle);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
			{
				//UnrecoverableError( "Failed to initialize OpenGL using GLAD." );

				// @@TODO: Change to real logging
				system("pause");
				exit(-1);
			}

			//init opengl / device
			GLsizei w = this->GetWindowWidth();
			GLsizei h = this->GetWindowHeight();
			glViewport(0, 0, w, h);
			// @@TODO: put these somewhere else
			//glEnable( GL_CULL_FACE );
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		//glfwSetFramebufferSizeCallback(m_WindowHandle, ResizeWindow);
		
		//TODO:
		//glfwSetCursorPosCallback(m_WindowHandle, OnCursorMove);
		//glfwSetScrollCallback(m_WindowHandle, OnCursorScroll);
		//glfwSetWindowCloseCallback(m_WindowHandle, OnWindowClose);

	}
	void GLWindow::UpdateGameWindow()
	{

	}
	void GLWindow::DisplayGameWindow()
	{

	}

	bool GLWindow::IsOpen()
	{

	}
	bool GLWindow::IsActive()
	{
		
	}
	void GLWindow::setActive(bool active)
	{

	}

	void GLWindow::ResizeWindow(GLWindow* windowhandle, int width, int height)
	{

	}

	unsigned GLWindow::GetWindowWidth()
	{

	}
	unsigned GLWindow::GetWindowHeight()
	{

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
