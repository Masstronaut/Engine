#pragma once

//std
#include <vector>
#include <memory> //shared ptr

//GLM
#include <glm/glm.hpp> //mat4

//ours
#include "Vertex.h"
#include "GL/GLProgram.h"  //temp

namespace Jellyfish
{
	//Allows for manipulation of individual mesh faces
	struct meshFace
	{
		// Number of indices defining this face.
		unsigned int mNumIndices;

		//Pointer to the indices array. Size of the array is given in numIndices.
		unsigned int* mIndices;
	};

	class iShader;
	class iTexture;
	class iMesh
	{
	public:
		
		virtual ~iMesh() 
		{

		}

		virtual void Draw() const = 0;
		virtual void AssignShader(GLProgram& shader) = 0;
	
	
	protected:
		iMesh(const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices, std::vector<std::shared_ptr<GLTexture>>& Textures) :
			m_Vertices(Vertices)
			, m_Indices(Indices)
			, m_Textures(Textures) //working on it
		{
			//called by derived classes only
		}

		std::vector<Vertex> m_Vertices; 
		std::vector<unsigned int> m_Indices;
		glm::mat4 m_Transform;

		//If the mesh is part of a tree structure, allows for relative transforms
		iMesh* m_Parent;

		// The number of primitives (triangles, polygons, lines) in this  mesh.
		unsigned int mNumFaces;
		// The faces the mesh is constructed from.
		meshFace* m_Faces;

		//mostly for debug, blank if the mesh was not named
		std::string m_Name;

		//Material properties for this mesh
		iShader* m_Material;
		//TEMP -- need to use the iShader above instead or id's into the resources
		GLProgram* m_shader;
		
		//this is awful..  we should pull from the resource library and only store id's
		std::vector<std::shared_ptr<iTexture>> m_Textures;


		//TODO: Add bone support
		//unsigned int mNumBones;
		//meshBone** mBones;
			
	private:
    };
}