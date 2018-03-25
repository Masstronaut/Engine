#pragma once
#include <GLM/glm.hpp>
#include <string>
#include "EventArena.hpp"
#include "Settings/WindowSettings.h"

enum class WindowState { minimized, maximized, restored, closed };
enum class CursorMode { normal, hidden, disabled };
class GLWindow : public EventArena {
public:
  struct Cursor {
    bool Clicked( ) const;
    int Held( ) const;
    bool Released( ) const;
    glm::dvec2 position;
    glm::dvec2 wheel;
    int pressed{ 0 };
    bool was_pressed{ false };
    CursorMode mode{ CursorMode::normal };
  };
#pragma region EventTypes
  struct EMouseMoved {
    glm::dvec2 newPosition{ 0.,0. };
    glm::dvec2 oldPosition{ 0.,0. };
  };
  struct EMouseScrolled {
    glm::dvec2 offset{ 0.,0. };
  };
  struct EMouseModeChanged {
    CursorMode newMode;
    CursorMode oldMode;
  };
  struct EMouseClicked {  };
  struct EMouseReleased { };
  struct EMouseHeld { int frames{ 0 }; };
  struct EWindowResized {
    glm::uvec2 newSize{ 0,0 };
    glm::uvec2 oldSize{ 0,0 };
  };
  struct EWindowStateChanged {
    WindowState newState;
    WindowState oldState;
  };
  struct ETitleChanged {
    std::string newTitle;
    std::string oldTitle;
  };
  struct EKeyPressed {
    int key{ 0 };
  };
#pragma endregion

  virtual ~GLWindow( ) { }
  const std::string& Title( ) const;
  const glm::uvec2& Size( ) const;
  const Cursor& Mouse( ) const;
  const glm::ivec2& Version( ) const;
  unsigned Height( ) const;
  unsigned Width( ) const;
  WindowState State( ) const;

  // Builder pattern window
  GLWindow& Title( const std::string &title );
  GLWindow& Size( const glm::uvec2 &value );
  GLWindow& Height( unsigned value );
  GLWindow& Width( unsigned value );
  GLWindow& MouseMode( CursorMode mode );
  GLWindow& State( WindowState mode );

protected:
  virtual void SetSizeImpl( const glm::uvec2 &size ) = 0;
  virtual void SetTitleImpl( const std::string &title ) = 0;
  virtual void SetMouseModeImpl( CursorMode mode ) = 0;
  virtual void SetStateImpl( WindowState state ) = 0;
  void SetMousePos( const glm::dvec2 &pos );
  void SetMouseMode( CursorMode mode );
  void SetMouseWheel( const glm::dvec2 &pos );

  std::string m_Title;
  glm::uvec2 m_Size{ g_InitialWindowWidth, g_InitialWindowHeight }; 
  glm::ivec2 m_Version{ 4,2 };
  WindowState m_State{ WindowState::restored };
  //EventArena m_Events;
private:
  Cursor m_Mouse;

};
