#pragma once
#include <glad/glad.h>
#include <array>
#include <iostream>
#include "GLShader.hpp"
GLShader::GLShader( const std::string &name )
  : Resource( name ) {
  auto ext{ this->Extension( ) };
  if( ext == "vert" ) {
    m_ShaderType = GL_VERTEX_SHADER;
  } else if( ext == "frag" ) {
    m_ShaderType = GL_FRAGMENT_SHADER;
  }
  this->Load( );
}
GLShader::GLShader( GLShader && shader )
: Resource(std::move(shader))
, m_ShaderID( std::move( shader.m_ShaderID ) ) 
, m_ShaderType( std::move( shader.m_ShaderType ) ) { 
  shader.m_ShaderID = 0;
  shader.m_ShaderType = 0;
}
GLShader::~GLShader( ) {
  this->Unload( );
}

bool GLShader::Reloadable( ) const {
  return true;
}
std::string GLShader::Directory( ) const {
  return ShaderPath( );
}
unsigned GLShader::Type( ) const { return m_ShaderType; }
unsigned GLShader::ID( ) const { return m_ShaderID; }

bool GLShader::LoadImpl( ) {
  m_ShaderID = glCreateShader( m_ShaderType );
  const GLchar * ShaderSrc{ Data( ).c_str( ) };
  glShaderSource( m_ShaderID, 1, &ShaderSrc, nullptr );
  glCompileShader( m_ShaderID );
  if( this->Check( ) ) {
    return true;
  } else {
    glDeleteShader( m_ShaderID );
    return false;
  }
}
void GLShader::UnloadImpl( ) {
  glDeleteShader( m_ShaderID );
  m_ShaderID = 0;
}
bool GLShader::Check( ) const {
  int success{ 0 };
  std::array<char, 512> info;
  glGetShaderiv( m_ShaderID, GL_COMPILE_STATUS, &success );
  if( !success ) {
    glGetShaderInfoLog( m_ShaderID, ( GLsizei )info.size( ), NULL, info.data( ) );
    std::cout << "Error: Shader compilation has failed for file: "
      << this->Filename( )
      << ".\n"
      << info.data( )
      << std::endl;
  }
  return success;
}
