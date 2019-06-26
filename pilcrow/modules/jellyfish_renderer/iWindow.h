#pragma once

// std
#include <string>

// GLM
#include <glm/glm.hpp>

// ours
#include "Utils/EventArena.hpp"  //event messaging

namespace Jellyfish {

enum class WindowState { minimized, maximized, restored, closed };
enum class CursorMode { normal, hidden, disabled };
enum class CursorClickedState { normal, held };

struct Cursor {
  // bool Clicked() const;
  // int Held() const;
  // bool Released() const;
  // this could be improved but not worth it probably
  glm::dvec2 position{0., 0.};
  glm::dvec2 wheel{0., 0.};
  // glfw only supports 10 buttons
  int        buttonStates[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int        buttonHeld[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  CursorMode mode{CursorMode::normal};
};

// struct EMouseScrolled {
//	glm::dvec2 offset{ 0.,0. };
//};
// struct EMouseModeChanged {
//	CursorMode newMode;
//	CursorMode oldMode;
//};

// struct EMouseReleased { };
// struct EMouseHeld { int frames{ 0 }; };
//

// void SetMousePos(const glm::dvec2 &pos);
// void SetMouseMode(CursorMode mode);
// void SetMouseWheel(const glm::dvec2 &pos);

class iWindow : public EventArena {
public:
#pragma region EventTypes

  // Window Events
  struct EWindowResized {
    glm::uvec2 newSize{0, 0};
    glm::uvec2 oldSize{0, 0};
  };
  struct EWindowStateChanged {
    WindowState newState;
    WindowState oldState;
  };
  // WIndow Input Events
  struct EKeyPressed {
    int key{0};
  };
  struct EMouseMoved {
    glm::fvec2 newPosition{0., 0.};
    glm::fvec2 oldPosition{0., 0.};
    Cursor     cursorData;
  };
  struct EMouseAction {
    int button{0};
    int action[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  };

#pragma endregion

  virtual ~iWindow() {}

  virtual void CreateGameWindow(unsigned width, unsigned height,
                                bool fullscreen, std::string title)
    = 0;
  virtual void UpdateGameWindow()  = 0;
  virtual void DisplayGameWindow() = 0;

  virtual bool IsOpen()   = 0;
  virtual bool IsActive() = 0;  // active tied to focused for now, i.e. pause on
                                // lose focus

  virtual void setActive(bool active)                   = 0;
  virtual void SetWindowTitle(const std::string &title) = 0;
  virtual void SetWindowState(WindowState state)        = 0;

  virtual void ResizeWindow(unsigned width, unsigned height) = 0;

  virtual unsigned GetWindowWidth()  = 0;
  virtual unsigned GetWindowHeight() = 0;

  virtual void PollEvents()                          = 0;
  virtual void PollInput(std::vector<int> &keyarray) = 0;

  virtual void FrameEnd() = 0;

  const glm::uvec2 & GetSize() const { return m_Size; }
  const std::string &GetTitle() const { return m_Title; }
  WindowState        GetState() const { return m_State; }

protected:
  std::string m_Title;
  glm::uvec2  m_Size{100, 100};
  WindowState m_State{WindowState::restored};
  bool        m_Active;
  bool        m_Open;
  Cursor      m_Cursor;

private:
};

}  // namespace Jellyfish
