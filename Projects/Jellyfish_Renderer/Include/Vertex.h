#pragma once
#include <glm/glm.hpp>

namespace Jellyfish
{
	struct Vertex 
	{
		Vertex() : 
			m_Position  ({ 0.f, 0.f, 0.f }),
			m_Normal    ({ 0.f, 0.f, 0.f }),
			m_Tangent   ({ 0.f, 0.f, 0.f }),
			m_Bitangent ({ 0.f, 0.f, 0.f }),
			m_Color     ({ 0.f, 0.f, 0.f, 1.f }) //Black
		{}

		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec3 m_Tangent;
		glm::vec3 m_Bitangent;
		glm::vec4 m_Color;

		union
		{
			//3D UVW support, (4D UV's are not supported by Assimp)
			glm::vec3 m_TexCoords;
			glm::vec3 m_UVs;
		};
		
	};

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