#pragma once
#include "iWindow.h" //base class

namespace Jellyfish
{
	class GLWindow : public iWindow
	{
	public:

		GLWindow() { }
		~GLWindow();

		void CreateGameWindow(unsigned width, unsigned height, bool fullscreen);
		void UpdateGameWindow();
		void DisplayGameWindow();

		bool IsOpen();
		bool IsActive(); //active tied to focused for now, i.e. pause on lose focus

		void setActive(bool active);

		unsigned GetWindowWidth();
		unsigned GetWindowHeight();

		void PollEvents();


	private:
		static void ResizeWindow(GLFWwindow* windowhandle, int width, int height);

		void SetWindowSize(const glm::uvec2 &size);
		void SetWindowTitle(const std::string &title);
		void SetWindowState(WindowState state);

		const glm::uvec2& GetWindowSize() const
		{
			return m_Size;
		}
		const std::string& GetWindowTitle() const
		{
			return m_Title;
		}
		WindowState GetWindowState() const
		{
			return m_State;
		}

		std::string m_Title;
		glm::uvec2 m_Size{ 100, 100 };
		WindowState m_State{ WindowState::restored };

		GLFWwindow * m_WindowHandle{ nullptr };

	};

} //namespace Jellyfish
