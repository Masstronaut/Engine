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

		//Todo: abstract assimp
		void ProcessNode(aiNode *node, const aiScene *scene);
		iMesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);

		//std::vector<std::shared_ptr<Texture>> LoadMaterialTextures( aiMaterial *mat, aiTextureType type );
		std::vector<iMesh*> m_Meshes;
	};
}