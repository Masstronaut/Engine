#include <iostream>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../include/settings/ResourceSettings.h"
#include "../include/Model.hpp"
#include "../include/RendererCommon.hpp"

Model::Model( const std::string & path ) 
  : Resource(path) {
  Load( IOType::binary );
}

Model::Model( const Model & model )
: Resource( model ) { 
  Load( );
}

Model::~Model( ) { }

void Model::Draw( GLProgram & shader ) const {
  for( auto &mesh : m_Meshes ) mesh.Draw( shader );
}

bool Model::Reloadable( ) const {
  return false;
}

std::string Model::Directory( ) const {
  std::string directory;
  directory.reserve(32);
  directory += g_ResourcePath;
  directory += "Models/";
  return directory;
}

bool Model::LoadImpl( ) {
  Assimp::Importer importer;
  importer.SetExtraVerbose( true );
  const aiScene *scene = importer.ReadFile( this->Path( ), aiProcess_Triangulate | aiProcess_FlipUVs );
  /*const aiScene *scene{ importer.ReadFileFromMemory( this->Data().c_str()
                                                   , this->Data().size() 
                                                   , aiProcess_Triangulate | aiProcess_FlipUVs
                                                   )
  };
  */
  if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
    std::cout << "Error: assimp failed to load the model \""
      << this->Name( )
      << "\". Info:\n"
      << importer.GetErrorString( )
      << std::endl;
    return false;
  }
  ProcessNode( scene->mRootNode, scene );
  return true;
}

void Model::UnloadImpl( ) { 

}

void Model::ProcessNode( aiNode * node, const aiScene * scene ) {
  for( unsigned i{ 0 }; i < node->mNumMeshes; ++i ) {
    aiMesh *mesh{ scene->mMeshes[ node->mMeshes[ i ] ] };
    m_Meshes.push_back( std::move( ProcessMesh( mesh, scene ) ) );
  }
  for( unsigned i{ 0 }; i < node->mNumChildren; ++i ) {
    ProcessNode( node->mChildren[ i ], scene );
  }
}

Mesh Model::ProcessMesh( aiMesh * mesh, const aiScene * scene ) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  for( unsigned i{ 0 }; i < mesh->mNumVertices; ++i ) {
    Vertex vert;
    vert.position.x = mesh->mVertices[ i ].x;
    vert.position.y = mesh->mVertices[ i ].y;
    vert.position.z = mesh->mVertices[ i ].z;
    vert.normal.x = mesh->mNormals[ i ].x;
    vert.normal.y = mesh->mNormals[ i ].y;
    vert.normal.z = mesh->mNormals[ i ].z;
    if( mesh->mTextureCoords[ 0 ] ) { // check if it has a texture
      vert.texCoords.x = mesh->mTextureCoords[ 0 ][ i ].x;
      vert.texCoords.y = mesh->mTextureCoords[ 0 ][ i ].y;
    } else {
      vert.texCoords = { 0.f, 0.f };
    }
    vertices.push_back( vert );
  }
  for( unsigned i{ 0 }; i < mesh->mNumFaces; ++i ) {
    aiFace face{ mesh->mFaces[ i ] };
    for( unsigned j{ 0 }; j < face.mNumIndices; ++j ) {
      indices.push_back( face.mIndices[ j ] );
    }
  }

  aiMaterial *material{ scene->mMaterials[ mesh->mMaterialIndex ] };
  auto mats{ LoadMaterialTextures( material, aiTextureType_DIFFUSE ) };
  textures.insert( textures.end( ), mats.begin( ), mats.end( ) );
  mats = LoadMaterialTextures( material, aiTextureType_SPECULAR );
  textures.insert( textures.end( ), mats.begin( ), mats.end( ) );
  
  return { vertices, indices, textures };
}



std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures( aiMaterial *mat, aiTextureType type ) {
  std::vector<std::shared_ptr<Texture>> textures;
  static std::unordered_map<std::string, std::shared_ptr<Texture>> loaded;
  for( unsigned int i = 0; i < mat->GetTextureCount( type ); ++i ) {
    aiString str;
    mat->GetTexture( type, i, &str );
    /*
    Texture texture;
    texture.id = TextureFromFile( str.C_Str( ), directory );
    texture.type = typeName;
    texture.path = str;
    */
    auto it{ loaded.find( str.C_Str( ) ) };
    if( it != loaded.end( ) ) {
      textures.push_back( it->second );
    } else {
      auto res = loaded.emplace( str.C_Str( ), std::make_shared<Texture>( str.C_Str( ) ) );
      textures.push_back( res.first->second );
      if( type == aiTextureType_DIFFUSE ) textures.back( )->Type( TextureType::diffuse );
      else if( type == aiTextureType_SPECULAR ) textures.back( )->Type( TextureType::specular );
    }
  }
  return textures;
}

