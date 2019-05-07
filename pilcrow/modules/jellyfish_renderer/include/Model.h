#pragma once

//std
#include <vector>
#include <limits>
#include <memory> //sharedptr

//GLM
#include <glm/glm.hpp>

//ours
#include <utils/include/Resource.hpp>


namespace Jellyfish
{
	class iMesh;
	
	class Model : public Resource
	{
	public:
		Model(const std::string &path);
		Model(const Model &model);
		~Model();
		void Draw() const;

		float GetScale(void)
		{
			return m_scalefactor;
		}

		void AssignMaterialToAllMeshes(unsigned int id);


	private:
		std::string Model::Directory() const override;
		virtual bool Reloadable() const final;
		virtual bool LoadImpl() final;
		virtual void UnloadImpl() final;

		//TODO: fix so not a vector, also maybe template this
		std::vector<iMesh*> m_Meshes;

		//uniform scalor of 1/boundingbox.max_range
		float m_scalefactor{ 0.f };
		//model space bounding box;
		glm::vec4 m_mn_vtx{ std::numeric_limits<float>::max() }, m_mx_vtx{ -std::numeric_limits<float>::max() };
	};
}