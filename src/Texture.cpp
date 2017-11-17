#pragma once
#include "Texture.hpp"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

Texture::Texture( const std::string &name )
  : Resource( name ) {
  this->Load( IOType::binary );
}
Texture::~Texture( ) {
  this->Unload( );
}
unsigned Texture::ID( ) const {
  return m_TextureID;
}

bool Texture::Reloadable( ) const {
  return true;
}
std::string Texture::Directory( ) const {
  return "../Resources/Textures/";
}
void Texture::Use( int TextureUnit ) const {
  glActiveTexture( GL_TEXTURE0 + TextureUnit );
  glBindTexture( GL_TEXTURE_2D, this->ID( ) );
}
TextureType Texture::Type( ) const {
  return m_Type;
}
Texture & Texture::Type( TextureType type ) {
  m_Type = type;
  return *this;
}
bool Texture::LoadImpl( ) {
  stbi_set_flip_vertically_on_load( true );
  
  unsigned char *data{ stbi_load_from_memory( ( const unsigned char* )this->Data( ).c_str( ), 
                                              this->Data( ).size( ), 
                                              &m_Width, 
                                              &m_Height, 
                                              &m_NumChannels, 
                                              0 
                                            )
  };


  if( data ) {
    std::cout << "Loaded texture " << Filename( ) << " successfully." << std::endl;
    GLenum format;
    if( m_NumChannels == 4 ) {
      format = GL_RGBA;
    }
    else if( m_NumChannels == 3 ) {
      format = GL_RGB;
    }
    else if( m_NumChannels == 1 ) {
      format = GL_RED;
    }
    glGenTextures( 1, &m_TextureID );
    glBindTexture( GL_TEXTURE_2D, this->ID( ) );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    stbi_image_free( data );
    return true;
  } else {
    std::cout << "Failed to load texture: " << this->Filename( ) << '.' << std::endl;
    return false;
  }
}
void Texture::UnloadImpl( ) {
  glDeleteTextures( 1, &m_TextureID );
  m_Width = m_Height = m_NumChannels = m_TextureID = 0;
}