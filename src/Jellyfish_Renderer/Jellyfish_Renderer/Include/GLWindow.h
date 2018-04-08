#pragma once
#include "iWindow.h" //base class

struct GLFWwindow;

namespace Jellyfish
{
	class GLWindow : public iWindow
	{
	public:

		GLWindow() { }
		~GLWindow();

		void CreateGameWindow(unsigned width, unsigned height, bool fullscreen, std::string title);
		void UpdateGameWindow();
		void DisplayGameWindow();

		bool IsOpen();
		bool IsActive(); //active tied to focused for now, i.e. pause on lose focus

		void setActive(bool active);
		void SetWindowTitle(const std::string &title);

		unsigned GetWindowWidth();
		unsigned GetWindowHeight();

		void PollEvents();

		void FrameEnd();

		void ResizeWindow(unsigned width, unsigned height);


		//const glm::uvec2& GetWindowSize() const
		//{
		//	return m_Size;
		//}
		//const std::string& GetWindowTitle() const
		//{
		//	return m_Title;
		//}
		//WindowState GetWindowState() const
		//{
		//	return m_State;
		//}


	private:
		static void ResizeWindow(GLFWwindow* windowhandle, int width, int height);
	
		void SetWindowState(WindowState state);


		std::string m_Title;
		glm::uvec2 m_Size{ 100, 100 };
		WindowState m_State{ WindowState::restored };

		GLFWwindow* m_WindowHandle{ nullptr };

	};

} //namespace Jellyfish
