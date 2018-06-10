// std
#include <iostream>
#include <unordered_map>

// glm
#include <glm/glm.hpp>

// ours
#include "utils/include/ResourceSettings.h"  //g_resourcePath
#include <utils/include/Resource.hpp>

#include "../include/AssimpLoaders.hpp"
#include "../include/Model.h"
#include "../include/iMesh.h"
#include "../include/iTexture.h"

// fix for multiplatform
#include "../include/GL/GLTexture.h"

namespace Jellyfish {
Model::Model(const std::string &path) : Resource(path) {
  // spacer for console readability
  std::cout << " " << std::endl;
  std::cout << "Initializing new Model Resource" << std::endl;
  Resource::Load(IOType::binary);
}

Model::Model(const Model &model) : Resource(model) { Load(); }

Model::~Model() = default;

void Model::Draw() const {
  // iMesh is abstract type, cannot use Draw() directly unless cast to real type
  for(auto &mesh : m_Meshes) {
    mesh.Draw();
  }
}

void Model::AssignShaderToAllMeshes(GLProgram &shader) {
  for(auto &mesh : m_Meshes) {
    mesh.AssignShader(shader);
  }
}

std::string Model::Directory() const {
  std::string directory;
  directory.reserve(32);
  directory += g_ResourcePath;
  directory += "Models/";
  return directory;
}

bool Model::Reloadable() const { return false; }

bool Model::LoadImpl() {
  // just use assimp loader functions for now
  std::cout << "Attempting to load Model: " << this->Path() << std::endl;
  bool loadstatus = Assimp_LoadModelFromFile(this->Path(), this->Name());

  if(loadstatus) {
    std::cout << "Model was successfully loaded." << std::endl;
  } else {
    std::cout << "ERROR! Model could not be loaded!  See Model::LoadImpl()"
              << std::endl;
  }

  // spacer for console readability
  std::cout << " " << std::endl;
  return loadstatus;
}

void Model::UnloadImpl() {}

std::vector<std::shared_ptr<GLTexture>>
Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type) {
  std::vector<std::shared_ptr<GLTexture>>                            textures;
  static std::unordered_map<std::string, std::shared_ptr<GLTexture>> loaded;

  for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);

    auto it{loaded.find(str.C_Str())};

    if(it != loaded.end()) {
      textures.push_back(it->second);
    } else {
      auto res
        = loaded.emplace(str.C_Str(), std::make_shared<GLTexture>(str.C_Str()));
      textures.push_back(res.first->second);

      if(type == aiTextureType_DIFFUSE) {
        textures.back()->Type(iTexture::TextureType::diffuse);
      } else if(type == aiTextureType_SPECULAR) {
        textures.back()->Type(iTexture::TextureType::specular);
      }
    }
  }

  return textures;
}
}  // namespace Jellyfish
