#include <glad/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include "RenderSystem.h"

void WindowManager::FrameStart( ) { 
  glfwPollEvents( );
  glClearColor( .3f, 0.2f, .3f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void WindowManager::FrameEnd( ) { 
  glfwSwapBuffers( window );
}

inline void WindowManager::ProcessInput( Camera &cam ) {
  if( window.KeyPressed( GLFW_KEY_ESCAPE ) ) {
    glfwSetWindowShouldClose( window, true );
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
