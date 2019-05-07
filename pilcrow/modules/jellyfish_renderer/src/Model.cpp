#pragma once

//std
#include <iostream>
#include <unordered_map>

//glm
#include <glm/glm.hpp>

//ours
#include <utils/include/Resource.hpp>
#include "utils/include/ResourceSettings.h" //g_resourcePath

#include "../include/Model.h"
#include "../include/AssimpLoaders.hpp"
#include "../include/iMesh.h"
#include "../include/iTexture.h"

//fix for multiplatform
#include "../include/GL/GLMesh.h"

namespace Jellyfish
{
	Model::Model(const std::string & path) : Resource(path)
	{
		//spacer for console readability
		std::cout << " " << std::endl;
		std::cout << "Initializing new Model Resource" << std::endl;
		Resource::Load(IOType::binary);
	}

	Model::Model(const Model & model)
		: Resource(model) {
		Load();
	}

	Model::~Model()
	{
		//delete all the meshes on this model
		for (auto m : m_Meshes)
			delete m;
	}

	void Model::Draw() const
	{
		//iMesh is abstract type, cannot use Draw() directly unless cast to real type
		for (auto &mesh : m_Meshes)
		{
			//TODO: figure out real type somehow
			//for now I'm just casting to GL to get on with this
		   static_cast<GLMesh*>(mesh)->Draw();
		}
	}

	void Model::AssignMaterialToAllMeshes(unsigned int id)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->AssignMaterial(id);
		}
	}

	std::string Model::Directory() const 
{
		std::string directory;
		directory.reserve(32);
		directory += g_ResourcePath;
		directory += "Models/";
		return directory;
	}

	bool Model::Reloadable() const
	{
		return false;
	}

	bool Model::LoadImpl() 
	{
		//just use assimp loader functions for now
		std::cout << "Attempting to load Model: " << this->Path() << std::endl;
		bool loadstatus = Assimp_LoadModelFromFile(this->Path(), this->Name(), this->m_Meshes, this->m_scalefactor);

		if(loadstatus)
			std::cout << "Model was successfully loaded." << std::endl;
		else
			std::cout << "ERROR! Model could not be loaded!  See Model::LoadImpl()" << std::endl;

		//spacer for console readability
		std::cout << " " << std::endl;
		return loadstatus;
	}

	void Model::UnloadImpl()
	{

	}



}