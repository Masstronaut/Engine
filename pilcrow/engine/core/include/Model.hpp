#pragma once
#include <vector>
#include <memory>

#include <Utils/include/Resource.hpp>

class GLProgram;
class Mesh;
class Texture;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
class Model : public Resource {
public:
  Model( const std::string &path );
  Model( const Model &model );
  ~Model( );
  void Draw( GLProgram &shader ) const;



private:
  virtual bool Reloadable( ) const final;
  virtual std::string Directory( ) const final;
  virtual bool LoadImpl( ) final;
  virtual void UnloadImpl( ) final;
  void ProcessNode( aiNode *node, const aiScene *scene );
  Mesh ProcessMesh( aiMesh *mesh, const aiScene *scene );
  std::vector<std::shared_ptr<Texture>> LoadMaterialTextures( aiMaterial *mat, aiTextureType type );

  std::vector<Mesh> m_Meshes;



};