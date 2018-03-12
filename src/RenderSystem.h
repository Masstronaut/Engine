#pragma once
#include "GLFWWindow.hpp"
#include "Camera.hpp"
#include "Entity/EntitiesWith.hpp"
#include "RenderComponents.h"
#include "Renderer.hpp"
#include "GLProgram.hpp"
#include "Model.hpp"
#include "Components/Transform.h"
#include "Camera.hpp"
struct WindowManager {
  void FrameStart( );
  void FrameEnd( );
  float Dt{ 0.f };
private:
  GLFWWindow window;
  void ProcessInput( Camera &cam );
};

struct TextRenderSystem {
  EntitiesWith<const RenderText> Entities;
  void Update() {
    for( const auto &entity : Entities ) {
      const RenderText &renderable{ entity.Get<const RenderText>( ) };
      gltr.Render( renderable.Text, renderable.Position, renderable.Color, renderable.Size );
    }
  }
  GLTextRenderer gltr{ "Text.sprog" };
};
struct FrameratePrinter {
  void PostProcess( ) {
    gltr.Render( "FPS: " + std::to_string( 1.f / Dt ), { 0.f,0.f }, { .5f,.8f,.2f } );
  }
  float Dt{ 0.f };
  GLTextRenderer gltr{ "Text.sprog" };
};
struct ModelRenderSystem {
  EntitiesWith<const Camera> Entities;
  void PreProcess( ) {
    if( Entities.cbegin( ) != Entities.cend( ) ) {
      camera = &Entities[ 0 ].Get<const Camera>( ); 
      projection = glm::perspective( glm::radians( camera->fov ), 800.f / 600.f, camera->nearplane, camera->farplane );
      program.SetUniform( "projection", projection );
      program.SetUniform( "view", camera->View( ) );
    }
  }
  void Process( const Model &model, const Transform &tf ) const {
    if( !camera ) return;
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate( modelMatrix, tf.pos );
    modelMatrix = glm::scale( modelMatrix, tf.scale );
    program.SetUniform( "model", modelMatrix );
    model.Draw( program );
  }
  glm::mat4 projection;
  const Camera *camera{ nullptr };
  mutable GLProgram program{ "Model.sprog" };
};