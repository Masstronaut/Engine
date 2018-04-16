#pragma once
#include <vector>
#include <memory> //shared ptr
#include <glm/glm.hpp> //mat4
#include "Vertex.h"

namespace Jellyfish
{
	class iShader;
	class iTexture;

	class iMesh
	{
	public:
		virtual ~iMesh() 
		{

		}

		virtual void Draw() const =0;

		void AssignVertices(std::vector<Vertex>& verts)
		{
			m_Vertices = verts;
		}
		void AssignIndices(std::vector<unsigned int>& indices)
		{
			m_Indices = indices;
		}
		void AssignTextures(std::vector<std::shared_ptr<iTexture>>& textures)
		{
			m_Textures = textures;
		}
	
	protected:
		std::vector<Vertex> m_Vertices; 
		std::vector<unsigned int> m_Indices;

		//later this can be stored as part of the material
		std::vector<std::shared_ptr<iTexture>> m_Textures;

		iShader* m_Material;

		iMesh* m_Parent; //for relative transforms
		glm::mat4 m_Transform;
			
	private:
    };
}