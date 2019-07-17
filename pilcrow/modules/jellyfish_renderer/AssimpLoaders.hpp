#pragma once

// std
#include <string>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// ours
#include "pilcrow/modules/jellyfish_renderer/Model.h"
#include "pilcrow/modules/jellyfish_renderer/Vertex.h"
#include "pilcrow/modules/jellyfish_renderer/iMesh.h"
#include "pilcrow/modules/jellyfish_renderer/iTexture.h"

// Temporary
#include "pilcrow/modules/jellyfish_renderer/GL/GLMesh.h"

namespace Jellyfish {

// Process an aiMesh, turning it into a generic mesh object which we can later
// convert into an API-specific mesh type (BO's & CB's).
// Currently just uses GL mesh, TODO: multi-platform this
GLMesh Model::Assimp_ProcessMesh(aiMesh &mesh, const aiScene &scene) {
  std::vector<Vertex>                     vertices;
  std::vector<unsigned int>               indices;
  std::vector<std::shared_ptr<GLTexture>> textures;  // TODO: fix for
                                                     // multuiplatform

  // Get Vertex Data
  for(unsigned i{0}; i < mesh.mNumVertices; ++i) {
    Vertex vert;

    // Positions
    if(mesh.HasPositions()) {
      vert.m_Position.x = mesh.mVertices[i].x;
      vert.m_Position.y = mesh.mVertices[i].y;
      vert.m_Position.z = mesh.mVertices[i].z;

      // get min and max for model's bounding box
      m_mn_vtx.x = glm::min(m_mn_vtx.x, mesh.mVertices[i].x);
      m_mn_vtx.y = glm::min(m_mn_vtx.y, mesh.mVertices[i].y);
      m_mn_vtx.z = glm::min(m_mn_vtx.z, mesh.mVertices[i].z);

      m_mx_vtx.x = glm::max(m_mn_vtx.x, mesh.mVertices[i].x);
      m_mx_vtx.y = glm::max(m_mn_vtx.y, mesh.mVertices[i].y);
      m_mx_vtx.z = glm::max(m_mn_vtx.z, mesh.mVertices[i].z);
    }
    // Normals
    if(mesh.HasNormals()) {
      vert.m_Normal.x = mesh.mNormals[i].x;
      vert.m_Normal.y = mesh.mNormals[i].y;
      vert.m_Normal.z = mesh.mNormals[i].z;
    }

    // Texture Coords
    if(mesh.mTextureCoords[0]) {  // check if it has a texture
      vert.m_TexCoords.x = mesh.mTextureCoords[0][i].x;
      vert.m_TexCoords.y = mesh.mTextureCoords[0][i].y;
    } else {
      vert.m_TexCoords = {0.f, 0.f, 0.f};
    }

    // Tangents & Bitangents
    if(mesh.HasTangentsAndBitangents()) {
      vert.m_Tangent.x = mesh.mTangents[i].x;
      vert.m_Tangent.y = mesh.mTangents[i].y;
      vert.m_Tangent.z = mesh.mTangents[i].z;

      vert.m_Bitangent.x = mesh.mBitangents[i].x;
      vert.m_Bitangent.y = mesh.mBitangents[i].y;
      vert.m_Bitangent.z = mesh.mBitangents[i].z;
    }

    vertices.push_back(vert);
  }

  // Get Index Data
  for(unsigned i{0}; i < mesh.mNumFaces; ++i) {
    aiFace face{mesh.mFaces[i]};

    for(unsigned j{0}; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Proces any materials
  if(mesh.mMaterialIndex >= 0) {
    aiMaterial *material{scene.mMaterials[mesh.mMaterialIndex]};

    auto mats{LoadMaterialTextures(material, aiTextureType_DIFFUSE)};
    textures.insert(textures.end(), mats.begin(), mats.end());

    mats = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), mats.begin(), mats.end());
  }

  // TODO: Determine Renderer type, and create the VBO's / Constant Buffers
  // Accordingly
  // currently uses GLMesh as return type
  std::cout << "Mesh Created with " << vertices.size() << " vertices."
            << std::endl;
  return {vertices, indices, textures};

}  // endfunc

// Recursive down node tree to process all mesh nodes
void Model::Assimp_ProcessNode(aiNode *node, const aiScene *scene) {
  for(unsigned i{0}; i < node->mNumMeshes; ++i) {
    aiMesh *mesh{scene->mMeshes[node->mMeshes[i]]};

    if(mesh && scene) {
      m_Meshes.push_back(std::move(Assimp_ProcessMesh(*mesh, *scene)));
      std::cout << "Assimp Node successfullly processed." << std::endl;
    }
  }
  for(unsigned i{0}; i < node->mNumChildren; ++i) {
    Assimp_ProcessNode(node->mChildren[i], scene);
  }

  return;
}  // endfunc

// Loads a model file into memory, which can then be accessed by string name
// with a lookup in the Resource Manager
bool Model::Assimp_LoadModelFromFile(const std::string &path,
                                     const std::string &name) {
  // create importer object
  Assimp::Importer importer;
  importer.SetExtraVerbose(true);
  std::cout << "Assimp Importer has been initialized." << std::endl;

  std::cout << "Assimp is parsing the file..." << std::endl;

  const aiScene *scene
    = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
                                | aiProcess_CalcTangentSpace
                                | aiProcess_GlobalScale);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    // error out if file couldn't be read
    std::cout << "Error: assimp failed to load the model \"" << name
              << "\". Info:\n"
              << importer.GetErrorString() << std::endl;
    return false;
  } else  // scene ok, process
  {
    std::cout << "File OK! Processing scene..." << std::endl;
    Assimp_ProcessNode(scene->mRootNode, scene);
    m_scalefactor
      = 1.0f
        / glm::max(m_mx_vtx.x - m_mn_vtx.x,
                   glm::max(m_mx_vtx.y - m_mn_vtx.y, m_mx_vtx.z - m_mn_vtx.z));
    std::cout << "SUCCESS - Model Loaded: " << name
              << "  with scalefactor: " << m_scalefactor << std::endl;
  }

  return true;
}  // endfunc

}  // end namespace Jellyfish
