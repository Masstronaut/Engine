#pragma once

//ours
#include "../include/GL/GLTexture.h"
#include "../include/Model.h"
#include "../include/Material.h"
#include "../include/iShader.h"
#include "../include/iTexture.h"

#include "../include/RenderResourceManager.h"

namespace Jellyfish
{
	Render_Resource_Manager::~Render_Resource_Manager()
	{
		//Delete Everything in the Resource Libraries

		//TODO: Test to make sure final derived class destructor still gets called

		for (auto i : m_models)
		{
			//delete the model -- meshes should be deleted in the model destructor
			delete i.second;
		}

		for (auto i : m_materials)
		{
			delete i.second;
		}

		for (auto t : m_textures)
		{
			delete t.second;
		}

		for (auto s : m_shaders)
		{
			delete s.second;
		}
	}

	unsigned int Render_Resource_Manager::GenerateUniqueID()
	{
		return ++m_unique_id_counter;
	}

	const Model* Render_Resource_Manager::GetModelByID(unsigned int id)
	{
		auto it{ m_models.find(id) };
		return it->second;
	}
	const Material* Render_Resource_Manager::GetMaterialByID(unsigned int id)
	{
		auto it{ m_materials.find(id) };
		return it->second;
	}
	const iShader* Render_Resource_Manager::GetShaderByID(unsigned int id)
	{
		auto it{ m_shaders.find(id) };
		return it->second;
	}
	const iTexture* Render_Resource_Manager::GetTextureByID(unsigned int id)
	{
		auto it{ m_textures.find(id) };
		return it->second;
	}

	template<typename T> 
	unsigned int Render_Resource_Manager::AddToManager(T* pResource)
	{
		unsigned int unique_ID = GenerateUniqueID();
		Add(unique_ID, pResource);
		return unique_ID;
	}	

	void Render_Resource_Manager::Add(unsigned int id, Model* pModel)
	{
		m_models.emplace(id, pModel);
	}
	void Render_Resource_Manager::Add(unsigned int id, Material* pMaterial)
	{
		m_materials.emplace(id, pMaterial);
	}
	void Render_Resource_Manager::Add(unsigned int id, iTexture* pTexture)
	{
		m_textures.emplace(id, pTexture);	
	}
	void Render_Resource_Manager::Add(unsigned int id, iShader* pShader)
	{
		m_shaders.emplace(id, pShader);
	}

}//end namespace Jellyfish