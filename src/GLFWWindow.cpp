#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLFWWindow.hpp"

std::unordered_map<GLFWwindow *, GLFWWindow *> GLFWWindow::m_CallbackHelpers{};

GLFWWindow::GLFWWindow() 
  : GLWindow( ) {
  if( glfwInit( ) ) {
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, this->Version( ).x );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, this->Version( ).y );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
  #if defined(__APPLE__)
    // required for MAC OSX support
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  #endif

	//Create the window and get a pointer to the handle
	if (g_StartFullscreen)
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* primaryMonMode = glfwGetVideoMode(primaryMonitor);

		//store in a modifyable object
		int tempWidth = primaryMonMode->width;
		int tempHeight = primaryMonMode->height;

		m_Size = glm::uvec2(tempWidth, tempHeight);
		m_WindowHandle = glfwCreateWindow(tempWidth, tempHeight, this->Title().c_str(), primaryMonitor, NULL);
		Size(m_Size);
	}
	else
		m_WindowHandle = glfwCreateWindow(this->Width(), this->Height(), this->Title().c_str(), NULL, NULL);

	
	if( !m_WindowHandle ) {
      // @@TODO: Change to real logging
      system( "pause" );
      exit( -1 );
    }
    m_CallbackHelpers[ m_WindowHandle ] = this;
    glfwMakeContextCurrent( m_WindowHandle );
    if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) ) {
      //UnrecoverableError( "Failed to initialize OpenGL using GLAD." );

      // @@TODO: Change to real logging
      system( "pause" );
      exit( -1 );
    }

	//init opengl / device
	GLsizei w = this->Width();
	GLsizei h = this->Height();
    glViewport( 0, 0, this->Width( ), this->Height( ) );
    // @@TODO: put these somewhere else
    //glEnable( GL_CULL_FACE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  }
  glfwSetFramebufferSizeCallback( m_WindowHandle, OnResize );
  glfwSetCursorPosCallback( m_WindowHandle, OnCursorMove );
  glfwSetScrollCallback( m_WindowHandle, OnCursorScroll );
  glfwSetWindowCloseCallback( m_WindowHandle, OnWindowClose );

}


GLFWWindow::operator GLFWwindow*( ) {
  return m_WindowHandle;
}

void GLFWWindow::OnResize( GLFWwindow *window, int w, int h ) {
  m_CallbackHelpers[ window ]->Size({ w,h });
}
void GLFWWindow::OnCursorMove( GLFWwindow *w, double x, double y ) {
  m_CallbackHelpers[ w ]->SetMousePos( { x,y } );
}
void GLFWWindow::OnCursorScroll( GLFWwindow *w, double x, double y ) {
  m_CallbackHelpers[ w ]->Emit( EMouseScrolled{ {x,y } } );
}
void GLFWWindow::OnWindowClose( GLFWwindow *w ) {
  m_CallbackHelpers[ w ]->State( WindowState::closed );
}

GLFWWindow::~GLFWWindow( ) {
  glfwDestroyWindow( m_WindowHandle );
  m_WindowHandle = nullptr;
}

bool GLFWWindow::KeyPressed( int key ) const {
  return glfwGetKey( m_WindowHandle, key) == GLFW_PRESS;
}

void GLFWWindow::SetSizeImpl( const glm::uvec2 & size ) {
  glfwMakeContextCurrent( m_WindowHandle );
  glViewport( 0, 0, size.x, size.y );
}

void GLFWWindow::SetTitleImpl( const std::string & title ) {
  glfwSetWindowTitle( m_WindowHandle, title.c_str( ) );
}

void GLFWWindow::SetMouseModeImpl( CursorMode mode ) {
  switch( mode ) {
  case CursorMode::disabled:
    glfwSetInputMode( m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    break;
  case CursorMode::normal:
    glfwSetInputMode( m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    break;
  case CursorMode::hidden:
    glfwSetInputMode( m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
    break;
  default:
    break;
  }
  this->SetMouseMode( mode );
}
void GLFWWindow::SetStateImpl( WindowState state ) {
  switch( state ) {
  case WindowState::maximized:
    glfwMaximizeWindow( m_WindowHandle );
    break;
  case WindowState::minimized:
    glfwIconifyWindow( m_WindowHandle );
    break;
  case WindowState::restored:
    glfwRestoreWindow( m_WindowHandle );
    break;
  case WindowState::closed:
    glfwSetWindowShouldClose( m_WindowHandle, true );
    break;
  default:
    return;
  }
  m_State = state;
}
