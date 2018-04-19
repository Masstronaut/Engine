#pragma once
#include <string>

#include <glad/include/glad.h>

#include <Utils/include/Resource.hpp>

std::string ShaderPath( );

class GLShader : public Resource {
public:
  GLShader( const std::string &name );
  GLShader( GLShader &&shader );
  ~GLShader( );

  virtual bool Reloadable( ) const override;
  virtual std::string Directory( ) const override;
  unsigned Type( ) const;
  unsigned ID( ) const;
protected:
  virtual bool LoadImpl( ) override;
  virtual void UnloadImpl( ) override;
  bool Check( ) const;

private:
  unsigned m_ShaderID{ 0 };
  unsigned m_ShaderType{ 0 };
};
