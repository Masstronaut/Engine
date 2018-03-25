#include <glad/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include "RenderSystem.h"

Camera* cam;

void MouseCallback2(const GLFWWindow::EMouseMoved &mme) {
  float sensitivity{ .05f };
  const float xoff{ sensitivity * (float)(mme.newPosition.x - mme.oldPosition.x) };
  const float yoff{ sensitivity * (float)(mme.newPosition.y - mme.oldPosition.y) };
  cam->yaw += xoff;
  cam->pitch -= yoff;
  if (cam->pitch > 89.9f) cam->pitch = 89.9f;
  if (cam->pitch < -89.9f) cam->pitch = -89.9f;
}
void ScrollCallback2(const GLFWWindow::EMouseScrolled &mse) {
  if (cam->fov >= 1.0f && cam->fov <= 70.0f)
    cam->fov -= (float)mse.offset.y;
  if (cam->fov <= 1.0f)
    cam->fov = 1.0f;
  if (cam->fov >= 70.0f)
    cam->fov = 70.0f;
}


WindowManager::WindowManager()
{
  window.MouseMode(CursorMode::disabled);
  window.Title("Engine v0.0");
  window.On( MouseCallback2 );
  window.On( ScrollCallback2 );
  glEnable(GL_DEPTH_TEST);
}
void WindowManager::FrameStart( ) {
  cam = &Entities[0].Get<Camera>();
  this->ProcessInput(*cam);
  glClearColor( 1.f, 1.f, 1.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
void WindowManager::FrameEnd( ) { 
  glfwSwapBuffers( window );
  glfwPollEvents();
}
inline void WindowManager::ProcessInput( Camera &cam ) {
  if( window.KeyPressed( GLFW_KEY_ESCAPE ) ) {
    window.State(WindowState::closed);
  }
  
  if( window.KeyPressed( GLFW_KEY_1 ) ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  if( window.KeyPressed( GLFW_KEY_2 ) ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth( 1.5f );
  }

  if( window.KeyPressed( GLFW_KEY_3 ) ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
  }
  float camSpeed{ 5.f * ( float )Dt };
  if( window.KeyPressed( GLFW_KEY_W ) )
    cam.position += camSpeed * cam.Front( );
  if( window.KeyPressed( GLFW_KEY_S ) )
    cam.position -= camSpeed * cam.Front( );
  if( window.KeyPressed( GLFW_KEY_A ) )
    cam.position -= cam.Right( ) * camSpeed;
  if( window.KeyPressed( GLFW_KEY_D ) )
    cam.position += cam.Right( ) * camSpeed;
  if( window.KeyPressed( GLFW_KEY_SPACE ) ) {
    cam.LookAt( { 0.f,0.f,0.f } );
  }

}
