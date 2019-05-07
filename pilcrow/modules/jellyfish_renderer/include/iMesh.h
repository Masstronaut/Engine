#pragma once

//std
#include <vector>
#include <memory> //shared ptr

//GLM
#include <glm/glm.hpp> //mat4

//ours
#include "Vertex.h"


namespace Jellyfish
{
	class Material;

	//Allows for manipulation of individual mesh faces
	struct meshFace
	{
		// Number of indices defining this face.
		unsigned int mNumIndices;

		//Pointer to the indices array. Size of the array is given in numIndices.
		unsigned int* mIndices;
	};

	class iMesh
	{
	public:
		
		virtual ~iMesh() 
		{

		}

		virtual void Draw() const = 0;
		
		//If/When we have more than one material per mesh we'll probably need to alter this logic
		void AssignMaterial(unsigned int material_id)
		{
			 m_material_id = material_id;
		}
	
	
	protected:
		iMesh(const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices) :
			m_Vertices(Vertices)
			, m_Indices(Indices)
		{
			//called by derived classes only -- should we maybe not have this?
			//TODO: investigate solutions
		}

		// ----------------------------------------------------------------------------------------
		//  Members
		// ----------------------------------------------------------------------------------------
		std::vector<Vertex> m_Vertices; 
		std::vector<unsigned int> m_Indices;
		glm::mat4 m_Transform;

		//If the mesh is part of a tree structure, allows for relative transforms
		//TODO: inplement tree structure in loading
		iMesh* m_parent;
		std::vector<iMesh*> m_children;

		// The number of primitives (triangles, polygons, lines) in this  mesh.
		unsigned int mNumFaces;
		// The faces the mesh is constructed from.
		meshFace* m_Faces;

		//mostly for debug, blank if the mesh was not named
		std::string m_Name;

		//Store ID -- future stuff: maybe do a touple with ID and material stringname for easier debugging?
		//per assimp - one material per mesh, assimp splits up meshes to ensure this
		unsigned int m_material_id;

		//TODO: Add bone support
		//unsigned int mNumBones;
		//meshBone** mBones;
			
	private:
    };
}