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
			std::cout << "Assimp Node successfullly processed." << std::endl;
		}
		for (unsigned i{ 0 }; i < node->mNumChildren; ++i)
		{
			Assimp_ProcessNode(node->mChildren[i], scene);
		}

		return;
	}

	GLMesh Model::Assimp_ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		if (mesh)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<std::shared_ptr<iTexture>> textures;


			for (unsigned i{ 0 }; i < mesh->mNumVertices; ++i)
			{
				Vertex vert;

				if (mesh->HasPositions())
				{
					vert.m_Position.x = mesh->mVertices[i].x;
					vert.m_Position.y = mesh->mVertices[i].y;
					vert.m_Position.z = mesh->mVertices[i].z;

					//get min and max for model's bounding box
					m_mn_vtx.x = glm::min(m_mn_vtx.x, mesh->mVertices[i].x);
					m_mn_vtx.y = glm::min(m_mn_vtx.y, mesh->mVertices[i].y);
					m_mn_vtx.z = glm::min(m_mn_vtx.z, mesh->mVertices[i].z);

					m_mx_vtx.x = glm::max(m_mn_vtx.x, mesh->mVertices[i].x);
					m_mx_vtx.y = glm::max(m_mn_vtx.y, mesh->mVertices[i].y);
					m_mx_vtx.z = glm::max(m_mn_vtx.z, mesh->mVertices[i].z);

				}
				if (mesh->HasNormals())
				{
					vert.m_Normal.x = mesh->mNormals[i].x;
					vert.m_Normal.y = mesh->mNormals[i].y;
					vert.m_Normal.z = mesh->mNormals[i].z;
				}
				if (mesh->mTextureCoords[0])
				{ // check if it has a texture
					vert.m_TexCoords.x = mesh->mTextureCoords[0][i].x;
					vert.m_TexCoords.y = mesh->mTextureCoords[0][i].y;
				}
				else {
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
			//currently uses GLMesh as return type

			std::cout << "Mesh Created with " << vertices.size() << " vertices." << std::endl;
			return { vertices, indices, textures };
		}
		else return GLMesh();
		
	}

	//Loads a model file into memory, which can then be accessed by string name 
	//with a lookup in the Resource Manager
	bool Model::Assimp_LoadModelFromFile(const std::string& path, const std::string& name)
	{
		//create importer object
		Assimp::Importer importer;
		importer.SetExtraVerbose(true);
		std::cout << "Assimp Importer has been initialized." << std::endl;

		std::cout << "Assimp is parsing the file..." << std::endl;

		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GlobalScale);
		
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
		{
			std::cout << "File OK! Processing scene..." << std::endl;
			Assimp_ProcessNode(scene->mRootNode, scene);
			m_scalefactor = 1.0 / glm::max(m_mx_vtx.x - m_mn_vtx.x, glm::max(m_mx_vtx.y - m_mn_vtx.y, m_mx_vtx.z - m_mn_vtx.z));
			std::cout << "SUCCESS - Model Loaded: " << name << "  with scalefactor: "  << m_scalefactor << std::endl;
		}
		
		return true;
    }


}

