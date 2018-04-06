#pragma once
#include <GLM/glm.hpp>
#include <string>

namespace Jellyfish
{

	enum class WindowState { minimized, maximized, restored, closed };
	enum class CursorMode { normal, hidden, disabled };

	//INPUT STUFF
	//struct Cursor {
	//	bool Clicked() const;
	//	int Held() const;
	//	bool Released() const;
	//	glm::dvec2 position;
	//	glm::dvec2 wheel;
	//	int pressed{ 0 };
	//	bool was_pressed{ false };
	//	CursorMode mode{ CursorMode::normal };
	//};
	//
	//struct EMouseMoved {
	//	glm::dvec2 newPosition{ 0.,0. };
	//	glm::dvec2 oldPosition{ 0.,0. };
	//};
	//struct EMouseScrolled {
	//	glm::dvec2 offset{ 0.,0. };
	//};
	//struct EMouseModeChanged {
	//	CursorMode newMode;
	//	CursorMode oldMode;
	//};
	//struct EMouseClicked {  };
	//struct EMouseReleased { };
	//struct EMouseHeld { int frames{ 0 }; };
	//
	struct EKeyPressed {
		int key{ 0 };
	};

	//void SetMousePos(const glm::dvec2 &pos);
	//void SetMouseMode(CursorMode mode);
	//void SetMouseWheel(const glm::dvec2 &pos);

	class iWindow {
	public:

#pragma region EventTypes


		struct EWindowResized {
			glm::uvec2 newSize{ 0,0 };
			glm::uvec2 oldSize{ 0,0 };
		};
		struct EWindowStateChanged {
			WindowState newState;
			WindowState oldState;
		};

#pragma endregion

		virtual ~iWindow() { }

		virtual void CreateGameWindow(unsigned width, unsigned height, bool fullscreen) = 0;
		virtual void UpdateGameWindow() = 0;
		virtual void DisplayGameWindow() = 0;

		virtual bool IsOpen() = 0;
		virtual bool IsActive() = 0; //active tied to focused for now, i.e. pause on lose focus

		virtual void setActive(bool active) = 0;

		virtual void ResizeWindow(unsigned width, unsigned height) = 0;

		virtual unsigned GetWindowWidth() = 0;
		virtual unsigned GetWindowHeight() = 0;

		virtual void PollEvents() = 0;


	protected:

		virtual void SetWindowSize(const glm::uvec2 &size) = 0;
		virtual void SetWindowTitle(const std::string &title) = 0;
		virtual void SetWindowState(WindowState state) = 0;

		const glm::uvec2& GetSize() const
		{
			return m_Size;
		}
		const std::string& GetTitle() const
		{
			return m_Title;
		}
		WindowState GetState() const
		{
			return m_State;
		}

		std::string m_Title;
		glm::uvec2 m_Size{ 100, 100 };
		WindowState m_State{ WindowState::restored };

	private:

	};

} //namespace Jellyfish
