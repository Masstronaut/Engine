#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "Model.hpp"

//base
struct Renderable {

};

struct RenderText : Renderable{
  std::string Text;
  glm::vec2 Position{ 0.f, 0.f };
  glm::vec3 Color{ .5f,.8f,.2f };
  float Size{ 1.f };
};

struct CModel : Renderable{
  CModel( const std::string &file ) : model( new Model( file ) ) { }
  CModel( const CModel& ) = default;
  CModel( CModel&& ) = default;
  CModel& operator=(const CModel &) = default;
  std::shared_ptr<Model> model;
};