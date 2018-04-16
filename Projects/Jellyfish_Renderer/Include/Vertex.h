#pragma once
#include <glm/glm.hpp>

namespace Jellyfish
{
	struct Vertex 
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
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