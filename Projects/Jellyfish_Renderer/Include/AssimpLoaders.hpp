#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "iMesh.h"
#include "iTexture.h"
#include "Vertex.h"

namespace Jellyfish
{
	/*
	void ProcessNode(aiNode * node, const aiScene * scene)
	{
		for (unsigned i{ 0 }; i < node->mNumMeshes; ++i)
		{
			aiMesh *mesh{ scene->mMeshes[node->mMeshes[i]] };
			m_Meshes.push_back(std::move(ProcessMesh(mesh, scene)));
		}
		for (unsigned i{ 0 }; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	iMesh* Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<iTexture>> textures;

		for (unsigned i{ 0 }; i < mesh->mNumVertices; ++i)
		{
			Vertex vert;
			vert.position.x = mesh->mVertices[i].x;
			vert.position.y = mesh->mVertices[i].y;
			vert.position.z = mesh->mVertices[i].z;
			vert.normal.x = mesh->mNormals[i].x;
			vert.normal.y = mesh->mNormals[i].y;
			vert.normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{ // check if it has a texture
				vert.texCoords.x = mesh->mTextureCoords[0][i].x;
				vert.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vert.texCoords = { 0.f, 0.f };
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

		//auto mats{ LoadMaterialTextures(material, aiTextureType_DIFFUSE) };
		//textures.insert(textures.end(), mats.begin(), mats.end());

		//mats = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		//textures.insert(textures.end(), mats.begin(), mats.end());

		//return { vertices, indices, textures };



		GLMesh* thismesh = new GLMesh;
		thismesh->AssignVertices(vertices);
		thismesh->AssignIndices(indices);
		thismesh->AssignTextures(textures);
		return thismesh;
	}

	//Loads a model file into memory, which can then be accessed by string name 
	//with a lookup in the Resource Manager
	void Assimp_LoadModelFromFile(const std::string& path, const std::string& name)
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
		}
		else //scene ok, process
			ProcessNode(scene->mRootNode, scene);
		
		return;
    }
*/

}

