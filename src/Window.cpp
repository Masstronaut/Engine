#include "Window.hpp"

//Getters
const std::string& GLWindow::Title( ) const { return m_Title; }
const glm::uvec2& GLWindow::Size( ) const { return m_Size; }
const GLWindow::Cursor& GLWindow::Mouse( ) const { return m_Mouse; }
const glm::ivec2& GLWindow::Version( ) const { return m_Version; }
unsigned GLWindow::Height( ) const { return m_Size.y; }
unsigned GLWindow::Width( ) const { return m_Size.x; }
WindowState GLWindow::State( ) const { return m_State; }

//Setters
GLWindow& GLWindow::Title( const std::string &title ) {
  if( this->Title( ) != title ) {
    std::string oldTitle{ this->Title( ) };
    SetTitleImpl( title );
    ETitleChanged event;
    event.newTitle = this->Title( );
    event.oldTitle = oldTitle;
    m_Events.Emit( event );
  }
  return *this;
}
GLWindow& GLWindow::Size( const glm::uvec2 &value ) {
  if( this->Size( ) != value ) {
    glm::uvec2 oldSize = this->Size( );
    SetSizeImpl( value );
    EWindowResized event;
    event.newSize = this->Size( );
    event.oldSize = oldSize;
    m_Events.Emit( event );
  }
  return *this;
}
GLWindow& GLWindow::Height( unsigned value ) {
  return this->Size( { this->Width( ), value } );
}
GLWindow& GLWindow::Width( unsigned value ) {
  return this->Size( { value, this->Height( ) } );
}


//Mouse Stuff
GLWindow& GLWindow::MouseMode( CursorMode mode ) {
  if( this->Mouse( ).mode != mode ) {
    CursorMode oldMode{ this->Mouse( ).mode };
    SetMouseModeImpl( mode );
    EMouseModeChanged event{ this->Mouse( ).mode, oldMode };
    m_Events.Emit( event );
  }
  return *this;
}
void GLWindow::SetMousePos( const glm::dvec2 &pos ) {
  const glm::dvec2 oldPosition{ this->Mouse( ).position };
  m_Mouse.position = pos;
  m_Events.Emit<EMouseMoved>( { pos, oldPosition } );
}
void GLWindow::SetMouseMode( CursorMode mode ) {
  m_Mouse.mode = mode;
}

bool GLWindow::Cursor::Clicked( ) const { return pressed && !was_pressed; }
int GLWindow::Cursor::Held( ) const {
  /* # of frames it has been held down, but 0 on the first frame. */
  // was_pressed is 0 if the mouse isn't held
  return was_pressed * pressed;
}
bool GLWindow::Cursor::Released( ) const { return !pressed && was_pressed; }

//State Machine
GLWindow& GLWindow::State( WindowState mode ) {
  if( this->State( ) != mode ) {
    WindowState oldState{ this->State( ) };
    this->SetStateImpl( mode );
    EWindowStateChanged event{ this->State( ), mode };
    m_Events.Emit( event );
  }
  return *this;
}