#pragma once
#include "Window.hpp"
#include <unordered_map>
struct GLFWwindow;
class GLFWWindow : public GLWindow {
public:
  GLFWWindow( );
  ~GLFWWindow( );

  bool KeyPressed( int key ) const;

  operator GLFWwindow*( );

private:
  void SetSizeImpl( const glm::uvec2 &size ) final;
  void SetTitleImpl( const std::string &title ) final;
  void SetMouseModeImpl( CursorMode mode ) final;
  void SetStateImpl( WindowState state ) final;
  GLFWwindow * m_WindowHandle{ nullptr };


  //@@TODO: solve this in a way that doesn't kill hotloading
  static std::unordered_map<GLFWwindow *, GLFWWindow *> m_CallbackHelpers;
  // Callbacks
  static void OnResize( GLFWwindow *, int, int );
  static void OnCursorMove( GLFWwindow *, double, double );
  static void OnCursorScroll( GLFWwindow *, double, double );
  static void OnWindowClose( GLFWwindow * );
};
