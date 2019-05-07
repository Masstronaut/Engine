#pragma once

//std
#include <string>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//ours
#include "Model.h"
#include "iMesh.h"
#include "iTexture.h"
#include "Vertex.h"
#include "Material.h"

#include "RenderResourceManager.h"

//Temporary
#include "GL/GLMesh.h"

namespace Jellyfish
{
	//used only to calculate bounding boxes
	struct min_max_verts
	{
		//min
		glm::vec3 m_min_vtx;

		//max
		glm::vec3 m_max_vtx;
	};

	std::vector<std::shared_ptr<GLTexture>> LoadMaterialTextures(aiMaterial *mat)
	{
		std::vector<std::shared_ptr<GLTexture>> textures;

		////for the number of textures of this type on the Material
		//for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
		//{
		//	//check by string name if this texture was loaded already in this loop
		//	aiString str;
		//	mat->GetTexture(type, i, &str);
		//	auto it{ loaded.find(str.C_Str()) };
		//
		//	//if loaded already, add it to the vector?
		//	if (it != loaded.end())
		//	{
		//		textures.push_back(it->second);
		//	}
		//	else
		//	{
		//		auto res = loaded.emplace(str.C_Str(), std::make_shared<GLTexture>(str.C_Str()));
		//		textures.push_back(res.first->second);
		//
		//		//we can construct our texture type directly with the ai_texturetype
		//		textures.back()->Type(iTexture::TextureType(type));
		//	}
		//}

		return textures;
	}

	//Process an aiMesh, turning it into a generic mesh object which we can later 
	//convert into an API-specific mesh type (BO's & CB's).  
	//Currently just uses GL mesh, TODO: multi-platform this
	iMesh* Assimp_ProcessMesh(aiMesh& mesh, const aiScene& scene, min_max_verts* bounding_verts)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		//Get Vertex Data
		for (unsigned i{ 0 }; i < mesh.mNumVertices; ++i)
		{
			Vertex vert;

			//Positions
			if (mesh.HasPositions())
			{
				vert.m_Position.x = mesh.mVertices[i].x;
				vert.m_Position.y = mesh.mVertices[i].y;
				vert.m_Position.z = mesh.mVertices[i].z;

				//get min and max for model's bounding box
				bounding_verts->m_min_vtx.x = glm::min(bounding_verts->m_min_vtx.x, mesh.mVertices[i].x);
				bounding_verts->m_min_vtx.y = glm::min(bounding_verts->m_min_vtx.y, mesh.mVertices[i].y);
				bounding_verts->m_min_vtx.z = glm::min(bounding_verts->m_min_vtx.z, mesh.mVertices[i].z);
				
				bounding_verts->m_max_vtx.x = glm::max(bounding_verts->m_max_vtx.x, mesh.mVertices[i].x);
				bounding_verts->m_max_vtx.y = glm::max(bounding_verts->m_max_vtx.y, mesh.mVertices[i].y);
				bounding_verts->m_max_vtx.z = glm::max(bounding_verts->m_max_vtx.z, mesh.mVertices[i].z);

			}
			//Normals
			if (mesh.HasNormals())
			{
				vert.m_Normal.x = mesh.mNormals[i].x;
				vert.m_Normal.y = mesh.mNormals[i].y;
				vert.m_Normal.z = mesh.mNormals[i].z;
			}

			//Texture Coords
			if (mesh.mTextureCoords[0])
			{ // check if it has a texture
				vert.m_TexCoords.x = mesh.mTextureCoords[0][i].x;
				vert.m_TexCoords.y = mesh.mTextureCoords[0][i].y;
			}
			else
			{
				vert.m_TexCoords = { 0.f, 0.f, 0.f };
			}

			//Tangents & Bitangents
			if (mesh.HasTangentsAndBitangents())
			{
				vert.m_Tangent.x = mesh.mTangents[i].x;
				vert.m_Tangent.y = mesh.mTangents[i].y;
				vert.m_Tangent.z = mesh.mTangents[i].z;


				vert.m_Bitangent.x = mesh.mBitangents[i].x;
				vert.m_Bitangent.y = mesh.mBitangents[i].y;
				vert.m_Bitangent.z = mesh.mBitangents[i].z;
			}
			
			vertices.push_back(vert);
		}

		//Get Index Data
		for (unsigned i{ 0 }; i < mesh.mNumFaces; ++i)
		{
			aiFace face{ mesh.mFaces[i] };

			for (unsigned j{ 0 }; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// ----------------------------------------------------------------------------------
		// Save this data in an iMesh
		// ----------------------------------------------------------------------------------

		//TODO: Determine Renderer type, and create the VBO's / Constant Buffers Accordingly
		//currently uses GLMesh as return type
		GLMesh* thismesh = new GLMesh(vertices, indices);
		std::cout << "Mesh Created with " << vertices.size() << " vertices." << std::endl;


		// ----------------------------------------------------------------------------------
		// Load associated Material and save id in this mesh  
		// ----------------------------------------------------------------------------------
		//TODO: this logic can possibly be moved to another thead

		//Get Material Data
		if (mesh.mMaterialIndex >= 0)
		{
			//Grab the material for this mesh -- per assimp, only ever one material per mesh
			aiMaterial* pMaterial{ scene.mMaterials[mesh.mMaterialIndex] };
			if (pMaterial)
			{
				//adds itself to the resource library
				//we can guarantee this material is only loaded once
				//by using the mMaterialIndex
				
				//Render_Resource_Manager& rrm = Render_Resource_Manager::get();
				//rrm.GetMaterialByID()
				
				//check if we already have that material loaded-

				//not loaded -- create a new one
				//Material* thisMaterial = new Material(mesh.mMaterialIndex);

				//read the material properties if there are any
				int props = pMaterial->mNumProperties;
				for (int i = 0; i < props; ++i)
				{
					//observe
					std::string key(pMaterial->mProperties[i]->mKey.C_Str());
					std::cout << "Observed Material Property: " << key << std::endl;
				}

				
				//LoadMaterialTextures could be overloaded and have different return types
			    //Load all the textures for this material
				//auto mats{ LoadMaterialTextures(pMaterial) };

				//save these somewhere and save the ID's in the Material

				
			}
		}		

		

		//auto-up-cast -- observe vals
		return thismesh;
		
	}//endfunc

	//Recursive down node tree to process all mesh nodes
	bool Assimp_ProcessNode(aiNode *node, const aiScene* scene, std::vector<iMesh*>& vec_meshes, min_max_verts* bounding_verts)
	{
		/// TODO:  Model meshes should really mirror the node graph structure
		/// Add meshes to a mesh tree/graph under the model instead of a vector
		/// This would allow us to operate on individual meshes in proper sequence, assuming the node structure is sensical
		/// Instead of a vector iteration, use a "find height of tree" type alg.
		/// When we process child nodes / meshes, we assign parent/child pointers

		//process all meshes at this node
		for (unsigned i{ 0 }; i < node->mNumMeshes; ++i)
		{
			aiMesh *mesh{ scene->mMeshes[node->mMeshes[i]] };

			if (mesh)
			{
				vec_meshes.push_back(std::move(Assimp_ProcessMesh(*mesh, *scene, bounding_verts))); // why deref? // investigate
				std::cout << "Assimp Node successfullly processed." << std::endl;
			}
		}
		//for all children, recurse
		for (unsigned i{ 0 }; i < node->mNumChildren; ++i)
		{
			Assimp_ProcessNode(node->mChildren[i], scene, vec_meshes, bounding_verts);
		}

		return true;
	}//endfunc

	//Loads a model file into memory, which can then be accessed by string name 
	//with a lookup in the Resource Manager
	bool Assimp_LoadModelFromFile(const std::string& path, const std::string& name, std::vector<iMesh*>& vec_meshes, float& scale)
	{
		//create importer object
		Assimp::Importer importer;
		importer.SetExtraVerbose(true);
		std::cout << "Assimp Importer has been initialized." << std::endl;

		std::cout << "Assimp is parsing the file..." << std::endl;

		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GlobalScale);
		
		
		//if scene is ok and can be read
		bool ret = false;
		if (scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode) 
		{
			std::cout << "File OK! Processing scene..." << std::endl;

			//track bounding box for whole model
		    //TODO - individual mesh bounding boxes would be cool
			min_max_verts* bounding_verts = new min_max_verts;

			if (Assimp_ProcessNode(scene->mRootNode, scene, vec_meshes, bounding_verts))
			{
				//scaling to the scalefactor should in theory fit the model to unit cube scale
				scale = 1.0f / glm::max(bounding_verts->m_max_vtx.x - bounding_verts->m_min_vtx.x, 
							   glm::max(bounding_verts->m_max_vtx.y - bounding_verts->m_min_vtx.y, 
										bounding_verts->m_max_vtx.z - bounding_verts->m_min_vtx.z));

				std::cout << "SUCCESS - Model Loaded: " << name << "  with scalefactor: " << scale << std::endl;

				ret = true;
			}
			else
			{
				//specific error case
				std::cout << "Error: assimp failed to process the nodes! \"" << std::endl;
			}

			delete bounding_verts;
		}

		//error out if file couldn't be read
		std::cout << "Error: assimp failed to load the model \""
				<< 
				name << "\". Info:\n"
				<< importer.GetErrorString()
				<< std::endl;

		return ret;
    }//endfunc

}//end namespace Jellyfish

