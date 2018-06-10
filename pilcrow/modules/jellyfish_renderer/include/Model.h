#pragma once

// std
#include <limits>
#include <memory>  //sharedptr
#include <vector>

// GLM
#include <assimp/material.h>
#include <glm/glm.hpp>

// ours
#include "GL/GLProgram.h"
#include <utils/include/Resource.hpp>

// assimp stuff
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Jellyfish {
class iMesh;
class iShader;

// fix
class GLTexture;
class GLMesh;

class Model : public Resource {
public:
  Model(const std::string &path);
  Model(const Model &model);
  ~Model();
  void Draw() const;

  float GetScale(void) { return m_scalefactor; }

  void AssignShaderToAllMeshes(GLProgram &shader);

private:
  std::string  Directory() const override;
  virtual bool Reloadable() const final;
  virtual bool LoadImpl() final;
  virtual void UnloadImpl() final;

  // TODO: fix so not GL
  std::vector<GLMesh> m_Meshes;
  std::vector<std::shared_ptr<GLTexture>>
  LoadMaterialTextures(aiMaterial *mat, aiTextureType type);

  // assimp laoders -- will probably move out of Model once we have a proper
  // memory management system
  void Assimp_ProcessNode(aiNode *node, const aiScene *scene);

  // TODO: fix hardcoded glmesh here
  GLMesh Assimp_ProcessMesh(aiMesh &mesh, const aiScene &scene);
  bool   Assimp_LoadModelFromFile(const std::string &path,
                                  const std::string &name);

  // std::vector<std::shared_ptr<Texture>> LoadMaterialTextures( aiMaterial
  // *mat, aiTextureType type );

  // uniform scalor of 1/boundingbox.max
  float m_scalefactor{0.f};
  // model space bounding box;
  glm::vec4 m_mn_vtx{std::numeric_limits<float>::max()},
    m_mx_vtx{-std::numeric_limits<float>::max()};
};
}  // namespace Jellyfish
