#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class VertexArray {
public:
  VertexArray( unsigned vertSize);
  virtual ~VertexArray( );
  void Bind( ) const;
  void Unbind( ) const;
  void BindAttribute( int location, int size, int type, bool normalized, unsigned offset );
protected:

private:
  unsigned m_VertexSize{ 0 };
  unsigned m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
};

class GLProgram;
class Mesh {

public:
  std::vector<Vertex> vertices;
  std::vector<unsigned> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  Mesh( const std::vector<Vertex>& verts, const std::vector<unsigned> &indices, std::vector<std::shared_ptr<Texture>> &textures );
  void Draw( GLProgram &shader ) const;
private:
  unsigned VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
  void Load( );
};

