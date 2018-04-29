#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "iMesh.h"
#include "iTexture.h"
#include "Vertex.h"

//Temporary
#include "GLMesh.h"

namespace Jellyfish
{
	
	void Model::Assimp_ProcessNode(aiNode * node, const aiScene * scene)
	{
		for (unsigned i{ 0 }; i < node->mNumMeshes; ++i)
		{
			aiMesh *mesh{ scene->mMeshes[node->mMeshes[i]] };
			m_Meshes.push_back(std::move(Assimp_ProcessMesh(mesh, scene)));
		}
		for (unsigned i{ 0 }; i < node->mNumChildren; ++i)
		{
			Assimp_ProcessNode(node->mChildren[i], scene);
		}
	}

	iMesh* Model::Assimp_ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<iTexture>> textures;

		for (unsigned i{ 0 }; i < mesh->mNumVertices; ++i)
		{
			Vertex vert;
			vert.m_Position.x = mesh->mVertices[i].x;
			vert.m_Position.y = mesh->mVertices[i].y;
			vert.m_Position.z = mesh->mVertices[i].z;
			vert.m_Normal.x = mesh->mNormals[i].x;
			vert.m_Normal.y = mesh->mNormals[i].y;
			vert.m_Normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{ // check if it has a texture
				vert.m_TexCoords.x = mesh->mTextureCoords[0][i].x;
				vert.m_TexCoords.y = mesh->mTextureCoords[0][i].y;
				vert.m_TexCoords.z = mesh->mTextureCoords[0][i].z;
			}
			else
			{
				vert.m_TexCoords = { 0.f, 0.f, 0.f };
			}
			vertices.push_back(vert);
		}

		for (unsigned i{ 0 }; i < mesh->mNumFaces; ++i)
		{
			aiFace face{ mesh->mFaces[i] };

			for (unsigned j{ 0 }; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial *material{ scene->mMaterials[mesh->mMaterialIndex] };

		//TODO: textures
		//auto mats{ LoadMaterialTextures(material, aiTextureType_DIFFUSE) };
		//textures.insert(textures.end(), mats.begin(), mats.end());

		//mats = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		//textures.insert(textures.end(), mats.begin(), mats.end());

		//return { vertices, indices, textures };


		//TODO: Determine Renderer type, and create the VBO's / Constant Buffers Accordingly
		GLMesh* thismesh = new GLMesh;
		//thismesh->AssignVertices(vertices);
		//thismesh->AssignIndices(indices);
		//thismesh->AssignTextures(textures);
		return thismesh;
	}

	//Loads a model file into memory, which can then be accessed by string name 
	//with a lookup in the Resource Manager
	bool Model::Assimp_LoadModelFromFile(const std::string& path, const std::string& name)
	{
		//create importer object
		Assimp::Importer importer;
		importer.SetExtraVerbose(true);
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			//error out if file couldn't be read
			std::cout << "Error: assimp failed to load the model \""
				<< 
				name << "\". Info:\n"
				<< importer.GetErrorString()
				<< std::endl;
			return false;
		}
		else //scene ok, process
			Assimp_ProcessNode(scene->mRootNode, scene);
		
		return true;
    }


}

