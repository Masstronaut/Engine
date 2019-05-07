#pragma once

#include <unordered_map> //resource manager

//Utils
#include <utils/include/Singleton.hpp>

namespace Jellyfish
{
	class Model;
	class Material;
	class iShader;
	class iTexture;

	SINGLETON(Render_Resource_Manager);
public:
	~Render_Resource_Manager();

	//This will see the most usage
	//remember to save the returned ID in the object itself
	//this should happen right after this function gets called
	template<typename T> 
	unsigned int AddToManager(T* pResource);
	
	template<typename T>
	unsigned int AddToManager(unsigned int index, T* pResource);


	const Model* GetModelByID(unsigned int id);
	const Material* GetMaterialByID(unsigned int id);
	const iShader* GetShaderByID(unsigned int id);
	const iTexture* GetTextureByID(unsigned int id);
	
	//mayyybe..
	//void GetResourcebyID(unsigned int id, RESOURCE_TYPE t)
	//{
	//	switch(t) ...
	//}

	
private:
	unsigned int GenerateUniqueID();

	void Add(unsigned int id, Model* pModel);
	void Add(unsigned int id, Material* pMaterial);
	void Add(unsigned int id, iTexture* pTexture);
	void Add(unsigned int id, iShader* pShader);

	static std::unordered_map<unsigned int, Model*> m_models;
	static std::unordered_map<unsigned int, Material*> m_materials;
	static std::unordered_map<unsigned int, iShader*> m_shaders;
	static std::unordered_map<unsigned int, iTexture*> m_textures;

	//arbitrary..  
	//TODO: use GUID ssytem
	unsigned int m_unique_id_counter = 100000;

}; //END CLASS Render_Resource_Manager

}//end namespace Jellyfish