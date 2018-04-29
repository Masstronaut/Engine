#pragma once
#include <vector>
#include <../../Modules/Utils/include/Resource.hpp>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Jellyfish
{
	class iMesh;
	class iShader;
	class Texture;

	class Model : public Resource
	{
	public:
		Model(const std::string &path);
		Model(const Model &model);
		~Model();
		void Draw() const;

	private:
		virtual bool Reloadable() const final;
		virtual bool LoadImpl() final;
		virtual void UnloadImpl() final;

		std::vector<iMesh*> m_Meshes;
		

		//assimp laoders -- will probably move out of Model once we have a proper
		//memory management system
		void Assimp_ProcessNode(aiNode * node, const aiScene * scene);
		iMesh* Assimp_ProcessMesh(aiMesh * mesh, const aiScene * scene);
		bool Assimp_LoadModelFromFile(const std::string& path, const std::string& name);

		//std::vector<std::shared_ptr<Texture>> LoadMaterialTextures( aiMaterial *mat, aiTextureType type );
	};
}