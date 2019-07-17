#pragma once

// ours
#include "pilcrow/modules/jellyfish_renderer/iMesh.h"
#include "pilcrow/modules/jellyfish_renderer/GL/GLTexture.h"

namespace Jellyfish {
class GLMesh : public iMesh {
public:
  GLMesh(const std::vector<Vertex> &              Vertices,
         const std::vector<unsigned> &            Indices,
         std::vector<std::shared_ptr<GLTexture>> &Textures);

  ~GLMesh() {}

  void Load();
  void Draw() const override;

  void AssignShader(GLProgram &shader) override;

private:
  unsigned VAO{0}, VBO{0}, EBO{0};
};
}  // namespace Jellyfish
