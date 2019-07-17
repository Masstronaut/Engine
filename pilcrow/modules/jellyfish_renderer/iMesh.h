#pragma once

// std
#include <memory>  //shared ptr
#include <vector>

// GLM
#include <glm/glm.hpp>  //mat4

// ours
#include "pilcrow/modules/jellyfish_renderer/GL/GLProgram.h"
#include "pilcrow/modules/jellyfish_renderer/Vertex.h"

namespace Jellyfish {
// Allows for manipulation of individual mesh faces
struct meshFace {
  // Number of indices defining this face.
  unsigned int mNumIndices;

  // Pointer to the indices array. Size of the array is given in numIndices.
  unsigned int *mIndices;
};

class iShader;
class iTexture;
class GLTexture;
class iMesh {
public:
  virtual ~iMesh() {}

  virtual void Draw() const                    = 0;
  virtual void AssignShader(GLProgram &shader) = 0;

protected:
  iMesh(const std::vector<Vertex> &              Vertices,
        const std::vector<unsigned> &            Indices,
        std::vector<std::shared_ptr<GLTexture>> &Textures)
    : m_Vertices(Vertices), m_Indices(Indices), m_Textures(Textures) {
    // called by derived classes only
  }

  std::vector<Vertex>       m_Vertices;
  std::vector<unsigned int> m_Indices;
  glm::mat4                 m_Transform;

  // If the mesh is part of a tree structure, allows for relative transforms
  iMesh *m_Parent;

  // The number of primitives (triangles, polygons, lines) in this  mesh.
  unsigned int mNumFaces;
  // The faces the mesh is constructed from.
  meshFace *m_Faces;

  // mostly for debug, blank if the mesh was not named
  std::string m_Name;

  // Material properties for this mesh
  iShader *m_Material;

  // TODO: fix for multiplatform
  std::vector<std::shared_ptr<GLTexture>> m_Textures;

  // TEMP
  GLProgram *m_shader;

  // TODO: Add bone support
  // unsigned int mNumBones;
  // meshBone** mBones;

private:
};
}  // namespace Jellyfish