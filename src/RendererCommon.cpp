#include <cstddef>
#include <glad/glad.h>
#include "RendererCommon.hpp"
#include "GLProgram.hpp"
Mesh::Mesh( const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices, std::vector<std::shared_ptr<Texture>>& Textures )
  : vertices( Vertices )
  , indices(Indices)
  , textures(Textures) {
  Load( );
}

void Mesh::Draw( GLProgram & shader ) const { 
  unsigned diffuse{ 1 }, specular{ 1 };
  for( unsigned i{ 0 }; i < textures.size( ); ++i ) {
    glActiveTexture( GL_TEXTURE0 + i );

    std::string name;
    switch( textures[ i ]->Type( ) ) {
    case TextureType::diffuse:
      name = "diffuse";
      name += std::to_string(diffuse++);
      break;
    case TextureType::specular:
      name = "specular";
      name += std::to_string( specular++ );
      break;
    }
    if( !shader.SetUniform( name, (int)i ) ) {
      //__debugbreak( );
    }
    
    glBindTexture( GL_TEXTURE_2D, textures[ i ]->ID( ) );
  }
  glActiveTexture( GL_TEXTURE0 );
  glBindVertexArray( VAO );
  glDrawElements( GL_TRIANGLES, (int)indices.size( ), GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
}

void Mesh::Load( ) {
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenBuffers( 1, &EBO );
  glBindVertexArray( VAO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, false, sizeof( Vertex ), nullptr );

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, false, sizeof( Vertex ), (void*)offsetof( Vertex, normal ) );
  glEnableVertexAttribArray( 2 );
  glVertexAttribPointer( 2, 2, GL_FLOAT, false, sizeof( Vertex ), ( void* )offsetof( Vertex, texCoords ) );

  
  glBufferData( GL_ARRAY_BUFFER, vertices.size( ) * sizeof( Vertex ), vertices.data( ), GL_STATIC_DRAW );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size( ) * sizeof( decltype( indices )::value_type ), indices.data( ), GL_STATIC_DRAW );

  glBindVertexArray( 0 );

}


VertexArray::VertexArray( unsigned vertSize )
  : m_VertexSize(vertSize) {
  glGenVertexArrays( 1, &m_VAO );
  glGenBuffers( 1, &m_VBO );
  glBindVertexArray( m_VAO );
  glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

  Unbind( );
}

VertexArray::~VertexArray( ) {
  glDeleteBuffers( 1, &m_VBO );
  glDeleteVertexArrays( 1, &m_VAO );

}

void VertexArray::Bind( ) const {
  glBindVertexArray( m_VAO );
}

void VertexArray::Unbind( ) const {
  glBindVertexArray( 0 );
}

void VertexArray::BindAttribute( int location, int size, int type, bool normalized, unsigned offset ) {
  Bind( );
  glVertexAttribPointer( location, size, type, normalized, m_VertexSize, reinterpret_cast<void*>(offset) );
  Unbind( );
}
