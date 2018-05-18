#pragma once

//GLM
#include <glm/glm.hpp>

namespace Jellyfish
{
	struct Vertex 
	{
		Vertex();

		glm::vec3 m_Position;
		glm::vec4 m_Color;
		glm::vec3 m_TexCoords;
		glm::vec3 m_Normal;
		glm::vec3 m_Tangent;
		glm::vec3 m_Bitangent;
	};

	//TODO:
	//Not being used yet
	class VertexArray 
	{
	public:
		VertexArray(unsigned vertSize);
		virtual ~VertexArray();
		
		void Bind() const;
		void Unbind() const;
		
		void BindAttribute(int location, int size, int type, bool normalized, void* offset);
	
	protected:
	
	private:
		unsigned m_VertexSize{ 0 };
		unsigned m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
	};
}