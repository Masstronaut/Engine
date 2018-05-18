#pragma once

//ours
#include "../iWindow.h" //base class

struct GLFWwindow;

namespace Jellyfish
{
	class GLWindow : public iWindow
	{
	public:
		GLWindow();
		~GLWindow();

		void CreateGameWindow(unsigned width, unsigned height, bool fullscreen, std::string title);
		void UpdateGameWindow();
		void DisplayGameWindow();

		bool IsOpen();
		bool IsActive(); //active tied to focused for now, i.e. pause on lose focus

		void setActive(bool active);
		void SetWindowTitle(const std::string &title);
		void SetWindowState(WindowState state);

		unsigned GetWindowWidth();
		unsigned GetWindowHeight();

		void PollEvents();
		void PollInput(std::vector<int>& keyarray);

		void FrameEnd();

		void ResizeWindow(unsigned width, unsigned height);

	private:
		//glfw callback functions
		static void Callback_ResizeWindow(GLFWwindow* windowhandle, int width, int height);
		static void Callback_WindowClose(GLFWwindow* windowhandle);
		static void Callback_WindowMove(GLFWwindow* windowhandle, int xpos, int ypos);
		static void Callback_CursorPosition(GLFWwindow* windowhandle, double xpos, double ypos);
		static void Callback_MouseButton(GLFWwindow* window, int button, int action, int mods);

		std::string m_Title{""};
		glm::uvec2 m_Size{100, 100};
		WindowState m_State{WindowState::restored};

		GLFWwindow* m_WindowHandle{ nullptr };
	};

	//Singleton Window
	static GLWindow* g_singleton_window;

} //namespace Jellyfish
